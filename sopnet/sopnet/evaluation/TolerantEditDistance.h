#ifndef SOPNET_EVALUATION_TOLERANT_EDIT_DISTANCE_H__
#define SOPNET_EVALUATION_TOLERANT_EDIT_DISTANCE_H__

#include <imageprocessing/ImageStack.h>
#include <pipeline/SimpleProcessNode.h>
#include <inference/Solution.h>
#include "Errors.h"
#include "Cell.h"

class TolerantEditDistance : public pipeline::SimpleProcessNode<> {

public:

	TolerantEditDistance();

private:

	typedef Cell<float>                             cell_t;
	typedef boost::shared_ptr<std::vector<cell_t> > cells_t;

	void updateOutputs();

	void extractCells();

	void enumerateCellLabels();

	// TOLERANCE FUNCTION RELATED -- TO BE OUTSOURCED

		// find all cells that can be relabeled
		std::vector<unsigned int> getRelabelCandidates();

		// find all offset locations for the given distance threshold
		std::vector<cell_t::Location> createNeighborhood();

		// search for all relabeling alternatives for the given cell and 
		// neighborhood
		std::set<float> getAlternativeLabels(const cell_t& cell, const std::vector<cell_t::Location>& neighborhood);

		// test, whether a voxel is surrounded by at least one other voxel with 
		// a different label
		bool isBoundaryVoxel(int x, int y, int z, ImageStack& stack);

	// END OF TOLERANCE FUNCTION RELATED

	void findBestCellLabels();

	void findErrors();

	void correctReconstruction();

	std::set<float>& getGroundTruthLabels();

	std::set<float>& getReconstructionLabels();

	void clear();

	void registerCell(float gtLabel, float recLabel, unsigned int cellIndex);

	void registerPossibleMatch(float gtLabel, float recLabel);

	std::set<float>& getPossibleMatchesByGt(float gtLabel);

	std::set<float>& getPossibleMathesByRec(float recLabel);

	void assignIndicatorVariable(unsigned int var, unsigned int cellIndex, float gtLabel, float recLabel);

	std::vector<unsigned int>& getIndicatorsByRec(float recLabel);

	std::vector<unsigned int>& getIndicatorsGtToRec(float gtLabel, float recLabel);

	void assignMatchVariable(unsigned int var, float gtLabel, float recLabel);

	unsigned int getMatchVariable(float gtLabel, float recLabel);

	// is there a background label?
	bool _haveBackgroundLabel;

	// the optional background labels of the ground truth and reconstruction
	float _gtBackgroundLabel;
	float _recBackgroundLabel;

	pipeline::Input<ImageStack> _groundTruth;
	pipeline::Input<ImageStack> _reconstruction;

	pipeline::Output<ImageStack> _correctedReconstruction;
	pipeline::Output<ImageStack> _splitLocations;
	pipeline::Output<ImageStack> _mergeLocations;
	pipeline::Output<ImageStack> _fpLocations;
	pipeline::Output<ImageStack> _fnLocations;
	pipeline::Output<Errors>     _errors;

	// list of all cells
	cells_t _cells;

	// map from rec labels to maps from gt label to cell indices
	std::map<float, std::map<float, std::vector<unsigned int> > > _cellsByRecToGtLabel;

	// the extends of the ground truth and reconstruction
	unsigned int _width, _height, _depth;

	// the distance threshold in nm
	float _maxDistanceThreshold;

	// the size of one voxel
	float _resolutionX;
	float _resolutionY;
	float _resolutionZ;

	// the distance threshold in pixels for each direction
	int _maxDistanceThresholdX;
	int _maxDistanceThresholdY;
	int _maxDistanceThresholdZ;

	// set of all ground truth labels
	std::set<float> _groundTruthLabels;

	// set of all reconstruction labels
	std::set<float> _reconstructionLabels;

	// all possible label matchings, from ground truth to reconstruction
	std::map<float, std::set<float> > _possibleGroundTruthMatches;

	// all possible label matchings, from reconstruction to ground truth
	std::map<float, std::set<float> > _possibleReconstructionMatches;

	// reconstruction label indicators by reconstruction label
	std::map<float, std::vector<unsigned int> > _indicatorVarsByRecLabel;

	// reconstruction label indicators by groundtruth label x reconstruction 
	// label
	std::map<float, std::map<float, std::vector<unsigned int> > > _indicatorVarsByGtToRecLabel;

	// (cell index, new label) by indicator variable
	std::map<unsigned int, std::pair<unsigned int, float> > _labelingByVar;

	// map from ground truth label x reconstruction label to match variable
	std::map<float, std::map<float, unsigned int> > _matchVars;

	// the number of indicator variables in the ILP
	unsigned int _numIndicatorVars;

	// the ILP variables for the number of splits and merges
	unsigned int _splits;
	unsigned int _merges;

	// the solution of the ILP
	pipeline::Value<Solution> _solution;
};

#endif // SOPNET_EVALUATION_TOLERANT_EDIT_DISTANCE_H__

