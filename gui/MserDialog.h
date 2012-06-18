#ifndef GUI_MSER_DIALOG_H__
#define GUI_MSER_DIALOG_H__

#include <boost/shared_ptr.hpp>

#include <gui/ContainerView.h>
#include <gui/Slider.h>
#include <gui/VerticalPlacing.h>
#include <imageprocessing/MserParameters.h>
#include <pipeline/all.h>

class MserDialog : public pipeline::ProcessNode {

public:

	MserDialog();

	~MserDialog();

private:

	/**
	 * A collector that bundles the outputs of the gui elements into a single
	 * MserParameters object.
	 */
	class ParametersCollector : public pipeline::SimpleProcessNode {

		public:

			ParametersCollector();

			~ParametersCollector();

		private:

			void updateOutputs();

			// inputs
			pipeline::Input<double> _delta;
			pipeline::Input<double> _minArea;
			pipeline::Input<double> _maxArea;
			pipeline::Input<double> _maxVariation;
			pipeline::Input<double> _minDiversity;
			pipeline::Input<bool>   _darkToBright;
			pipeline::Input<bool>   _brightToDark;

			// the graph cut parameters in a single object
			pipeline::Output<MserParameters> _parameters;
	};

	// a slider controlling the potts weight
	boost::shared_ptr<gui::Slider> _deltaSlider;
	boost::shared_ptr<gui::Slider> _minAreaSlider;
	boost::shared_ptr<gui::Slider> _maxAreaSlider;
	boost::shared_ptr<gui::Slider> _maxVariationSlider;
	boost::shared_ptr<gui::Slider> _minDiversitySlider;

	// a container of gui elements to control the fields of the parameters
	// object
	boost::shared_ptr<gui::ContainerView<gui::VerticalPlacing> > _gui;

	// a collector that creates the parameters object from the output of the gui
	// elements
	boost::shared_ptr<ParametersCollector> _parametersCollector;

	signals::Slot<pipeline::Modified> _modified;

	signals::Slot<pipeline::Updated>  _updated;
};

#endif // GUI_MSER_DIALOG_H__

