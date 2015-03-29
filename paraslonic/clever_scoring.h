#ifndef UI_CLEVER_SCORING_H
#define UI_CLEVER_SCORING_H

#include "../global.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class UI_Mainwindow;


struct SPW // experimental 4 peak wave eposidos
{
	SPW() {reset();}
	long long start,  end, duration;
	void reset() { start = 0; end = 0; duration = 0; }
};


struct STag // 4 non standard scorings
{
	int id;
	string name;
	string short_name;
	static int max_id;
	// fun
	STag() { id = -1; }
	// operators
	STag(const STag& tag) { id = tag.id; name = tag.name; }
	bool operator == (const string& name) 
		{ return this->name == name; }
	bool operator == (const STag& tag) 
	{ return strcmp(this->name.c_str(), tag.name.c_str()); }
	bool operator == (const int& id) const
	{ return this->id == id; }
	bool operator <(const STag& tag2) const
		{ return strcmp(this->name.c_str(), tag2.name.c_str()); }
};

struct epoch_stat 
{
	vector<double> spectrum;
	double eeg_amplitude;
	double motion_mrs;
	bool motion; // have motion signal 
	epoch_stat(){ motion = false; motion_mrs = 0; eeg_amplitude = 0; }
};

struct epochStats
{
	vector<epoch_stat> stats;
	vector<double> mspectrum;
	vector<double> mspectrum_20, mspectrum_80; // percentiles
	double mean_mot;
};


//////////////////////////////////////////////////////////////////////////
// class CleverScoring

class CleverScoring
{
public:
	CleverScoring(UI_Mainwindow* window = NULL);
	~CleverScoring();
	void init(UI_Mainwindow* window);
	void populateVoidScoring();
	// load/save
	void loadScoring(char* filename);
	void loadPWScoring(char* filename);
	void saveScoring(char* filename);
	void savePWScoring(char* filename);
	// tags
	void addNewTag(string name);
	STag tagByName(string name);
	void add_epoch_tagStat(epoch_stat stat, STag tag);
	int tagStatCount(string name);
	epochStats getTagStats(string name);
	vector<STag> epochTags(int epoch);
	bool epochHaveTag(int epoch, string tagname);
	bool switchTag(int epoch, string tagname, bool forceOn = false);
	void epochRemoveTag(int epoch, string tagname);
	//
	void set_bins(int bins);
	int get_bins();
	void add_epoch_stat(epoch_stat stat, TStage stage);
	void digest(); // calculate statistics
	bool wakeByMotion(epoch_stat& stat);
	bool classify(epoch_stat& stat, TStage& stage);
	bool classifyHist(epoch_stat& stat, TStage& stage, int index); // make use of history
	bool classifyTag(epoch_stat& stat, STag& tag);
	void save_txt();
	string epochName(int epoch);
	void log(string message);
	void log(string message, int num);
private: // scoring
	void digestTags(); 
	void calculate_motion_levels();
	void calculate_median_spectrums();
	void calculate_median_tagSpectrums();
	double calculate_spectrum_distance(vector<double>& spectrum1, vector<double>& spectrum2);
	double check_spectrum_distance(vector<double>& spectrum, vector<double>& mspectrum,
												vector<double>& mspectrum_90, vector<double>& mspectrum_10,
												double doverit_c = 1);
	bool check_REM();
	bool check_SW();
	bool check_W();

/////////// data
public: // statistics
	vector<epoch_stat> epoch_stat_REM;
	vector<epoch_stat> epoch_stat_SW;
	vector<epoch_stat> epoch_stat_W;
	vector<epoch_stat> epoch_stat_PW;
	vector< vector<double> > spectrums_rem, spectrums_sw, spectrums_w, spectrums_pw;
	vector<double> mspectrum_rem, mspectrum_sw, mspectrum_w, spectrum_pw; // median spectrum
	vector<double> mspectrum_rem_10, mspectrum_sw_10, mspectrum_w_10; // 10 percentile
	vector<double> mspectrum_rem_90, mspectrum_sw_90, mspectrum_w_90; // 90 percentile
	double mean_mot_rem, mean_mot_sw, mean_mot_w, mean_mot_pw; 
	double max_mot_rem, max_mot_sw, max_mot_w, max_mot_pw; 
public: // tags
	vector<STag> tags;
	vector< vector<STag> > epochsTags; // need 2 populate
	map<STag, epochStats> tagStats;
public: // scorins
	vector<SPW> vPW; // expiremental  
	int *scoring; // 2do: make vector
	bool *expert, *outliers; 
	int epoch_count;
	bool mspectrum_remExist, mspectrum_swExist, mspectrum_wExist;
	double rem_doverit_c, sw_doverit_c, w_doverit_c, tag_doverit_c;
private:
	int rem_count, sw_count, w_count, bins; 
	epoch_stat current;
	UI_Mainwindow* window;
};


#endif