#include "clever_scoring.h"
#include "../mainwindow.h"
#include "inttypes.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>
int STag::max_id = -1;

const char* log_file_name = "clever_scoring_log.txt";

CleverScoring::CleverScoring(UI_Mainwindow* window)
{
	rem_doverit_c = 0.5; w_doverit_c = 0.2; sw_doverit_c = 0.8; // one std dev is ok
	motion_sleep_threshold = 0;
	tag_doverit_c = 1;
	epoch_count = 0;
	scoring = NULL;
	expert = NULL;
	outliers = NULL;
	this->window = window;
	// reset log 
	FILE* logfile = fopen(log_file_name, "w");
	fclose(logfile);
	log("CleverScoring::CleverScoring()");
}

CleverScoring::~CleverScoring()
{
	delete[] scoring;
	delete[] expert;
	delete[] outliers;
	log("CleverScoring::~CleverScoring()");

}

void CleverScoring::init(UI_Mainwindow* window)
{
	log("init start");
	this->window = window;
	if(scoring != NULL) { delete[] scoring; scoring = NULL; }
	if(expert != NULL) { delete[] expert; expert = NULL; }
	if(outliers != NULL) { delete[] outliers; outliers = NULL; }
	epochsTags.clear();
	epoch_count = 0;
	epoch_stat_W.clear();	
	epoch_stat_SW.clear();
	epoch_stat_REM.clear();
	spectrums_rem.clear();
	spectrums_sw.clear();
	spectrums_w.clear();

	mean_mot_rem = 0; mean_mot_sw = 0; mean_mot_w = 0;
	max_mot_rem = 0; max_mot_sw = 0; max_mot_w = 0;
	rem_count = 0; sw_count = 0; w_count = 0; bins = 0;
	mspectrum_remExist = false, mspectrum_swExist = false, mspectrum_wExist = false;

	vPW.clear();

	log("init complete");
}

void CleverScoring::populateVoidScoring()
{
	log("populateVoidScoring");
	log("epoch count: ", epoch_count);
	if(scoring != NULL) { delete[] scoring; scoring = NULL; }
	if(expert != NULL) { delete[] expert; expert = NULL; }
	scoring = new int[epoch_count]; 
	expert = new bool[epoch_count]; 
	outliers = new bool[epoch_count]; 
	epochsTags.resize(epoch_count);
	for(int i = 0; i< epoch_count; i++)  { scoring[i] = s_NA; expert[i] = false; outliers[i] = false; }
}

void CleverScoring::clearStats()
{
	epoch_stat_SW.clear();
	epoch_stat_REM.clear();
	epoch_stat_W.clear();
	epoch_stat_PW.clear();
}

void CleverScoring::loadScoring(char* filename)
{
	log("loadScoring");
	FILE *sf; 
	sf = fopen(filename, "r");
	if(!sf) return;
	log("..opened");
	char buf[1024];
	int d;
	fscanf(sf, "%s\t%d", buf, &d);
	if(strcmp(buf, "[epoch]")==0)
	{ window->epoch_duration = d; }
	epoch_count = 0;
	fscanf(sf, "%s\t%d", buf, &d);
	if(strcmp(buf, "[epoch_count]")==0)
	{ epoch_count = d; window->epoch_count = epoch_count; }
	if(epoch_count < 1) return;
	populateVoidScoring();

	log("memory allocated");

	char stage[1024];
	int i = 0;
	fscanf(sf, "%s", buf);
	if(strcmp(buf, "[scoring]")==0)
	{
		int is_expert = 0, pw = 0, is_outlier = 0;
		STag pwtag = tagByName("pw");
		while (fscanf(sf, "%s\t%d\t%d\t%d", stage, &is_expert, &pw, &is_outlier) >= 1)
		{
			if(epoch_count != 0 && i >= epoch_count) break;
			if(strcmp(stage, na_string) == 0)
				scoring[i]=(int)s_NA;
			else if(strcmp(stage, sw_string) == 0)
				scoring[i]=(int)s_SW;
			else if(strcmp(stage, rem_string) == 0)
				scoring[i]=(int)s_REM;
			else if(strcmp(stage, wake_string) == 0)
				scoring[i]=(int)s_Wake;
			expert[i] = is_expert;
			outliers[i] = is_outlier;
			if(pw == 1) epochsTags[i].push_back(pwtag);
			// annotate in edf
			char annotation[MAX_ANNOTATION_LEN];
			if(is_expert) sprintf(annotation, "%s*",stage);
			if(pw) sprintf(annotation, "%s+%s",annotation, "pw");
			else sprintf(annotation, "%s",stage);
			if(is_outlier) sprintf(annotation, "%s[%s]",annotation, "out");
			annotation[MAX_ANNOTATION_LEN] = 0;
			window->annotate(annotation, i);
			i++;
		}
	}
	fclose(sf);
	log("annotation loaded");
}

void CleverScoring::loadPWScoring(char* filename)
{
	log("load pw scoring");
	FILE *sf = NULL; 
	sf = fopen(filename, "r");
	if(sf==NULL) return;
	log("..opened");
	char buf[1024],buf2[1024];
	int d;
	fscanf(sf, "%s\t%d", buf, &d);
	if(strcmp(buf, "[count]")==0) vPW.reserve(d);
	SPW spw; 

	while (fscanf(sf, "%I64d\t%I64d\t%s\t%s", &spw.start, &spw.end, buf, buf2) == 4)
	{
		vPW.push_back(spw);
	}

}


void CleverScoring::saveScoring(char* filename)
{
	FILE *sf; 
	sf = fopen(filename, "w");
	if(!sf) return;

	fprintf(sf, "[epoch]\t%d\n", window->epoch_duration);
	fprintf(sf, "[epoch_count]\t%d\n", epoch_count);
	fprintf(sf, "[scoring]\n");
	for(int i = 0; i < epoch_count; i++)
	{
		int stage = (int)scoring[i];
		const char* annot_text = NULL;
		switch(stage)
		{
		case s_NA:
			annot_text = na_string; break;
		case s_REM:
			annot_text = rem_string; break;
		case s_SW:
			annot_text = sw_string; break;
		case s_Wake:
			annot_text = wake_string; break;
		default:
			annot_text = "NA";
		}
		int e = 0;
		if(expert != NULL) e = (int)expert[i];
		int pw = (int)epochHaveTag(i, "pw");
		fprintf(sf, "%s\t%d\t%d\t%d\n", annot_text, e, pw, (int)outliers[i]);		
	}
	fclose(sf);
}

void CleverScoring::savePWScoring(char* filename)
{
	FILE *sf; 
	sf = fopen(filename, "w");
	if(!sf) return;
	fprintf(sf, "[count]\t%d\n", (int)vPW.size());
	vector<SPW>::iterator it = vPW.begin();
	for(;it != vPW.end();it++)
	{
		fprintf(sf, "%I64d\t%I64d\t", it->start, it->end);
		// 2 humans
		char str_start[256], str_end[256];
		long long l_tmp = it->start;
		sprintf(str_start, "%i:%02i:%02i.%04i",
			(int)((l_tmp / TIME_DIMENSION)/ 3600LL),
			(int)(((l_tmp / TIME_DIMENSION) % 3600LL) / 60LL),
			(int)((l_tmp / TIME_DIMENSION) % 60LL),
			(int)(((l_tmp % TIME_DIMENSION) / 1000LL)));
		l_tmp = it->end;
		sprintf(str_end, "%i:%02i:%02i.%04i",
			(int)((l_tmp / TIME_DIMENSION)/ 3600LL),
			(int)(((l_tmp / TIME_DIMENSION) % 3600LL) / 60LL),
			(int)((l_tmp / TIME_DIMENSION) % 60LL),
			(int)(((l_tmp % TIME_DIMENSION) / 1000LL)));
		fprintf(sf, "%s\t%s\n", str_start, str_end);
	}


	fclose(sf);
}

//////////////////////////////////////////////////////////////////////////
// tags

void CleverScoring::addNewTag(string name)
{
	// if exist do nothing
	vector<STag>::iterator it = find(tags.begin(), tags.end(), name);
	if(it != tags.end()) return;

	STag tag;
	int id = STag::max_id++;
	tag.id = id;
	tag.name = name;
	tags.push_back(tag);
}

STag CleverScoring::tagByName(string name)
{
	vector<STag>::iterator it = find(tags.begin(), tags.end(), name);
	if(it == tags.end()) return STag();
	return *it;
}

// 2do removeTag


//////////////////////////////////////////////////////////////////////////

void CleverScoring::set_bins(int bins)
{
	this->bins = bins;
	spectrums_rem.resize(bins);
	spectrums_w.resize(bins);
	spectrums_sw.resize(bins);

	mspectrum_w.resize(bins);
	mspectrum_sw.resize(bins);
	mspectrum_rem.resize(bins);

	mspectrum_w_10.resize(bins);
	mspectrum_sw_10.resize(bins);
	mspectrum_rem_10.resize(bins);
 
	mspectrum_w_90.resize(bins);
	mspectrum_sw_90.resize(bins);
	mspectrum_rem_90.resize(bins);

}

int CleverScoring::get_bins()
{
	return bins;
}

void CleverScoring::add_epoch_stat(epoch_stat stat, TStage stage)
{
	if(bins == 0) set_bins(stat.spectrum.size());
	switch(stage)
	{
	case s_SW:
		epoch_stat_SW.push_back(stat);
		break;
	case s_Wake:
		epoch_stat_W.push_back(stat);
		break;
	case s_REM:
		epoch_stat_REM.push_back(stat);
		break;
	}
	if(bins == 0) set_bins(stat.spectrum.size());
}

void CleverScoring::add_epoch_tagStat(epoch_stat stat, STag tag)
{
	if(bins == 0) set_bins(stat.spectrum.size());
	(tagStats[tag]).stats.push_back(stat);
}

int CleverScoring::tagStatCount(string name)
{
	STag tag = tagByName(name);
	return (tagStats[tag]).stats.size();
}

epochStats CleverScoring::getTagStats(string name)
{
	STag tag = tagByName(name);
	return tagStats[tag];
}

bool CleverScoring::epochHaveTag(int epoch, string tagname)
{
	vector<STag>::iterator it;
	it = find(epochsTags[epoch].begin(), epochsTags[epoch].end(), tagname);
	if (it == epochsTags[epoch].end()) return false;
	return true;
}

bool CleverScoring::switchTag(int epoch, string tagname, bool forceOn) // true if switched on
{
	if(!forceOn && epochHaveTag(epoch, tagname)) { epochRemoveTag(epoch, tagname); return false; }
	else { epochsTags[epoch].push_back(tagByName(tagname)); return true; }
}

void CleverScoring::epochRemoveTag(int epoch, string tagname)
{
	vector<STag>::iterator it = find(epochsTags[epoch].begin(), epochsTags[epoch].end(), tagname);
	if(it == epochsTags[epoch].end()) return;
	epochsTags[epoch].erase(it);
}



void CleverScoring::digest()
{
	log("digest");
	rem_count = epoch_stat_REM.size();
	sw_count = epoch_stat_SW.size();
	w_count = epoch_stat_W.size();
	calculate_motion_levels();
	calculate_median_spectrums();
	digestTags();
}

void CleverScoring::digestTags()
{
	log("digest_tags");
	log("tags: ", tags.size());
	calculate_median_tagSpectrums();
	// distribution of dists in expert scoring
	STag pw = tagByName("pw");
	log("expert dists:");
	for(int s = 0; s < tagStats[pw].stats.size(); s++)
	{
		double dist = check_spectrum_distance(tagStats[pw].stats[s].spectrum,
			tagStats[pw].mspectrum,tagStats[pw].mspectrum_80,tagStats[pw].mspectrum_20);
		log("",dist);
	}
	// 2do: motion
}


bool CleverScoring::wakeByMotion(epoch_stat& stat)
{
	if(stat.motion && stat.motion_mrs > motion_sleep_threshold) 
	{
		//log("wake by motion: ", (int)current.motion_mrs);
		return true;
	}
	return false;
}

bool CleverScoring::classify(epoch_stat& stat, TStage& stage)
{
	log("classify");
	current = stat;
	double dSW = -1, dREM = -1, dW = -1;
	
	if(wakeByMotion(stat)) { stage = s_Wake; return true; }
	
	dSW = abs(check_spectrum_distance(current.spectrum, mspectrum_sw, mspectrum_sw_90, mspectrum_sw_10, sw_doverit_c)); 
	dREM = abs(check_spectrum_distance(current.spectrum, mspectrum_rem, mspectrum_rem_90, mspectrum_rem_10, sw_doverit_c)); 
	dW = abs(check_spectrum_distance(current.spectrum, mspectrum_w, mspectrum_w_90, mspectrum_w_10, sw_doverit_c)); 

	if(dSW < dREM && dSW < dW) { stage = s_SW; return true; }	
	if(dW < dREM && dW < dSW) { stage = s_Wake; return true; }
	
	if(dREM < dSW && dREM < dW) { stage = s_REM; return true; }
	
	stage = s_NA;
	return false;
	
	/*// OLD Version
	if(mspectrum_swExist && check_SW()) { stage = s_SW; return true; }
	if(mspectrum_remExist && check_REM()) { stage = s_REM; return true; }
	if(mspectrum_wExist && check_W()) { stage = s_Wake; return true; }

	stage = s_NA;
	return false;*/
}

bool CleverScoring::classifyHist(epoch_stat& stat, TStage& stage, int index)
{
	log("classify with history");
	current = stat;
	double dSW = -1, dREM = -1, dW = -1;

	

	dSW = abs(check_spectrum_distance(current.spectrum, mspectrum_sw, mspectrum_sw_90, mspectrum_sw_10, sw_doverit_c)); 
	dREM = (check_spectrum_distance(current.spectrum, mspectrum_rem, mspectrum_rem_90, mspectrum_rem_10, sw_doverit_c)); 
	bool remMinus = (dREM < 0); dREM = abs(dREM);
	dW = abs(check_spectrum_distance(current.spectrum, mspectrum_w, mspectrum_w_90, mspectrum_w_10, sw_doverit_c)); 

	if(dSW < dREM && dSW < dW) { stage = s_SW; return true; }	
	if(dW < dREM && dW < dSW) { stage = s_Wake; return true; }

	// rem or maybe not?
	if(stat.motion && stat.motion_mrs > max_mot_rem) 
	{
		if(dSW < dW) { stage = s_SW; return true; } else { stage = s_Wake; return true; }
	}

	// Try history
	int epoch_dur = window->epoch_duration;
	int history_len = 80/epoch_dur;  
	int hi_min = (index > history_len )? index - history_len : 0;

	int hsw = 0, hrem = 0, hwake = 0;
	for(int i = hi_min; i < index; i++) 
	{
		switch(scoring[i]) { case s_SW: hsw++; break; case s_REM: hrem++; break; case s_Wake: hwake++; break; }
	}
	double hnonwake = (hsw+hrem)/(hsw+hrem+hwake);
	if(dREM > 0 && dREM < dSW && dREM < dW && hnonwake >= 0.8) { stage = s_REM; return true; } // 2do: move 2 config
	if(dSW < dW) { stage = s_SW; return true; } else { stage = s_Wake; return true; }
	
	stage = s_NA;
	return false;
}



bool CleverScoring::classifyTag(epoch_stat& stat, STag& tag)
{
	log("classifyTag");
	vector<STag>::iterator it = tags.begin();
	bool classified = false;
	while(it != tags.end())
	{
		log((*it).name);
		// epochStats* = tagStats[(*it)]; // 2do
		if(tagStats[(*it)].mspectrum.size() != bins) { it++; continue; }
		double dist = check_spectrum_distance(stat.spectrum, tagStats[(*it)].mspectrum, 
											tagStats[(*it)].mspectrum_80, tagStats[(*it)].mspectrum_20, tag_doverit_c);
		log("dist: ", dist);
		if(dist > 0) {tag = *it; classified = true; }
		it++; // 2do select minimum dist!!
	}
	return classified;
}

void addMotion(vector<epoch_stat> stats, vector<double>& motions){
	 for(int i = 0; i < stats.size(); i++) motions.push_back(stats[i].motion_mrs);
}

void CleverScoring::calculate_motion_levels()
{
	// calculate motion levels
	vector <double> motionSleep;
	addMotion(epoch_stat_REM, motionSleep);
	addMotion(epoch_stat_SW, motionSleep);
	sort(motionSleep.begin(), motionSleep.end());
	int count = (int)motionSleep.size();
	motion_sleep_threshold = motionSleep[(int)(count*0.95)];

	// write current threshold 2 file
	char fname[1024];
	sprintf(fname, "%s.motion_scoring.txt", window->edfheaderlist[0]->filename);
	FILE* file = fopen(fname, "w");
	fprintf(file, "%f\n", (float)motion_sleep_threshold);
	fclose(file);
}

void CleverScoring::calculate_median_spectrums()
{
	mspectrum_remExist = false, mspectrum_swExist = false, mspectrum_wExist = false;
	log("calculate_median_spectrums");
	log("bins: ", bins);
	if(rem_count + sw_count + w_count == 0) return;
	for(int i  = 0; i < bins; i++)
	{
		spectrums_rem[i].resize(rem_count);
		spectrums_sw[i].resize(sw_count);
		spectrums_w[i].resize(w_count);
	}
	for(int j  = 0; j < rem_count; j++)
	{
		for(int i  = 0; i < bins; i++)
		spectrums_rem[i][j] = epoch_stat_REM[j].spectrum[i];
	}
	for(int j  = 0; j < sw_count; j++)
	{
		for(int i  = 0; i < bins; i++)
			spectrums_sw[i][j] = epoch_stat_SW[j].spectrum[i];
	}
	for(int j  = 0; j < w_count; j++)
	{
		for(int i  = 0; i < bins; i++)
			spectrums_w[i][j] = epoch_stat_W[j].spectrum[i];
	}

	// calc median spectrums

	int cw = w_count;
	int csw = sw_count;
	int crem = rem_count;

	for(int i  = 0; i < bins; i++) 
	{
		if(cw > 0)
		{
			sort(spectrums_w[i].begin(), spectrums_w[i].end());
			mspectrum_w[i] = spectrums_w[i][cw/2];
			mspectrum_w_10[i] = spectrums_w[i][cw/5];
			mspectrum_w_90[i] = spectrums_w[i][(cw*8)/10];
			mspectrum_wExist = true;
		}
		if(crem > 0)
		{
			sort(spectrums_rem[i].begin(), spectrums_rem[i].end());
			mspectrum_rem[i] = spectrums_rem[i][crem/2];
			mspectrum_rem_10[i] = spectrums_rem[i][crem/10];
			mspectrum_rem_90[i] = spectrums_rem[i][(crem*9)/10];
			mspectrum_remExist = true;
		}
		if(csw > 0)
		{
			sort(spectrums_sw[i].begin(), spectrums_sw[i].end());
			mspectrum_sw[i] = spectrums_sw[i][csw/2];
			mspectrum_sw_10[i] = spectrums_sw[i][csw/10];
			mspectrum_sw_90[i] = spectrums_sw[i][(csw*9)/10];
			mspectrum_swExist = true;
		}
	}
}

void CleverScoring::calculate_median_tagSpectrums()
{
	log("calculate_median_tagSpectrums");
	vector<STag>::iterator tagit;
	tagit = tags.begin();
	while(tagit != tags.end())
	{
		int epochs = tagStats[(*tagit)].stats.size();
		if (epochs == 0) {tagit++; continue; }
		log((*tagit).name.c_str(), epochs);

		tagStats[(*tagit)].mspectrum.resize(bins);
		tagStats[(*tagit)].mspectrum_20.resize(bins);
		tagStats[(*tagit)].mspectrum_80.resize(bins);


		vector< vector<double> > spectrums;
		spectrums.resize(bins);
		for(int i  = 0; i < bins; i++) spectrums[i].resize(epochs);
		for(int s = 0; s < epochs; s++)
		{
			for(int i  = 0; i < bins; i++)
				spectrums[i][s] = tagStats[(*tagit)].stats[s].spectrum[i];
		}
		for(int i  = 0; i < bins; i++) 
		{
			 
			tagStats[(*tagit)].mspectrum_20[i] = spectrums[i][epochs/5];
			tagStats[(*tagit)].mspectrum_80[i] = spectrums[i][(epochs*4)/5];
		}
		tagit++;
	}
}


void CleverScoring::save_txt()
{
	log("save txt");
	
	FILE *sf; 
	sf = fopen("scoring_statistics.txt", "w");
	fprintf(sf, "%d\n", bins);
	fprintf(sf, "Wake\n");
	if(mspectrum_wExist)
	{
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_w[i]); fprintf(sf, "\n");
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_w_10[i]); fprintf(sf, "\n");
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_w_90[i]); fprintf(sf, "\n");
	}

	if(mspectrum_swExist)
	{
		fprintf(sf, "SW\n");
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_sw[i]); fprintf(sf, "\n");
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_sw_10[i]); fprintf(sf, "\n");
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_sw_90[i]); fprintf(sf, "\n");
	}

	if(mspectrum_remExist)
	{
		fprintf(sf, "REM\n");
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_rem[i]); fprintf(sf, "\n");
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_rem_10[i]); fprintf(sf, "\n");
		for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", mspectrum_rem_90[i]); fprintf(sf, "\n");
	}
	// by tag
	fprintf(sf, "max sw motion:\t\f\n",max_mot_sw);
	fprintf(sf, "max rem motion:\t\f\n",max_mot_rem);
	fprintf(sf, "max rem motion:\t\f\n",max_mot_w);

	vector<STag>::iterator tagit;
	tagit = tags.begin();
	while(tagit != tags.end())
	{
		fprintf(sf, "tag: %s\n", (*tagit).name.c_str());
		if(tagStats[(*tagit)].mspectrum.size() == bins)
		{
			for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", tagStats[(*tagit)].mspectrum[i]); fprintf(sf, "\n");
			for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", tagStats[(*tagit)].mspectrum_20[i]); fprintf(sf, "\n");
			for(int i  = 0; i < bins; i++) fprintf(sf, "%f\t", tagStats[(*tagit)].mspectrum_80[i]); fprintf(sf, "\n");
		}
		tagit++;
	}
	fclose(sf);
}

string CleverScoring::epochName(int epoch)
{
	if(scoring == NULL) return "";
	string name;
	switch(scoring[epoch])
	{
	case s_NA:
		name = na_string; break;
	case s_REM:
		name = rem_string; break;
	case s_SW:
		name = sw_string; break;
	case s_Wake:
		name = wake_string; break;
	default:
		name = "";
	}
	if(expert != NULL && expert[epoch]) name += "*"; 
	if(epochHaveTag(epoch, "pw")) name += "+pw";
	return name;
}

vector<STag> CleverScoring::epochTags(int epoch)
{
	if (epochsTags.size() < epoch) return vector<STag>();
	return epochsTags[epoch];
}



//////////////////////////////////////////////////////////////////////////

double CleverScoring::calculate_spectrum_distance(vector<double>& spectrum1, vector<double>& spectrum2)
{
	double dist = 0;
	for(int i = 0; i < bins; i++)
	{
		double d = spectrum2[i] - spectrum1[i];
		dist += d*d;
	}
	dist = sqrt(dist);
	return dist;
}

double CleverScoring::check_spectrum_distance(vector<double>& spectrum, vector<double>& mspectrum,
											vector<double>& mspectrum_90, vector<double>& mspectrum_10,
											double doverit_c)
{
	// i have median (mspectrum) and procentiles for each bin
	// so lets make use of them. Calculate distance 2 procentiles and dist 2 median, 
	// divide and see. Returns distance to median. Negative if out of percentiles*c
	doverit_c = 1.5;
	bool ok = true;
	int outs = 0;
	double sumCDist = 0; // dist to median
	for(int i = 0; i < bins; i++)
	{
		double d = (spectrum[i] - mspectrum[i]);
		double dm10 = (mspectrum[i] - mspectrum_10[i]);
		double dm90 = (mspectrum_90[i] - mspectrum[i]);
		
		//sumCDist += (2*abs(d))/(dm90-dm10);
		sumCDist += abs(d);

		if(d < 0) if ((-d) > dm10*doverit_c) outs++;
		if(d > 0) if (d > dm90*doverit_c) outs++;
	}
	if(outs > bins*0.2) ok = false;
	return ok? sumCDist : -sumCDist;
}

//////////////////////////////////////////////////////////////////////////

bool CleverScoring::check_REM()
{
	if(current.motion)
	{
		if(current.motion_mrs < max_mot_rem)
			if (check_spectrum_distance(current.spectrum, mspectrum_rem, 
										mspectrum_rem_90, mspectrum_rem_10, rem_doverit_c) > 0)
			{ return true; }
	}
	else if(check_spectrum_distance(current.spectrum, mspectrum_rem, 
									mspectrum_rem_90, mspectrum_rem_10, rem_doverit_c) > 0)
	{ return true; }
}

bool CleverScoring::check_SW()
{
	if(current.motion)
	{
		if(current.motion_mrs < max_mot_sw) // 2bd - quantilize
			if (check_spectrum_distance(current.spectrum, mspectrum_sw, 
										mspectrum_sw_90, mspectrum_sw_10, sw_doverit_c) > 0)
			{ return true; }
	}
	else if (check_spectrum_distance(current.spectrum, mspectrum_sw, 
							 		mspectrum_sw_90, mspectrum_sw_10, sw_doverit_c) > 0)
			{ return true; }
}

bool CleverScoring::check_W()
{
	log("check W");
	if(current.motion)
	{
		if( current.motion_mrs > max(max_mot_sw, max_mot_rem)) 
		{
			log("wake by motion: ", (int)current.motion_mrs);
			return true;
		}
	}
	if(check_spectrum_distance(current.spectrum, mspectrum_w, 
								mspectrum_w_90, mspectrum_w_10, w_doverit_c) > 0)
	{ log("wake by spectrum"); return true; }
}

void CleverScoring::log(string message)
{
	FILE* logfile = fopen(log_file_name, "a");
	fprintf(logfile, "%s\n", message.c_str());
	fclose(logfile);
}

void CleverScoring::log(string message, int num)
{
	stringstream ss;
	ss << message << num;
	log(ss.str());
}
