//
// LIO LIBRARIES
// simple framework for neural net and genetics
// (LIO stands for "LIfe-On-a-chip" -- who cares about acronyms, anyway...)
//
// started: 20090126
//
// contributors: 
//    Nicolas Bredeche (bredeche(at)lri.fr)
//    Evert Haasdijk (e.haasdijk(at)few.vu.nl)
//
// history (major revision):
//    20090130.15h26 [nicolas]: release candidate 1 
//			(perceptron, MLP, activation function, tanh lookup table, fixed-point integer type, obstacle avoidance behavior)
//    20090203.16h14 [nicolas]: added to symbrion repository
//			working example using perceptron for wall avoidance. ad hoc weights, no hidden layer, lookup-table for sigmoid/tanh. 
//    20090203.16h57 [evert]: added genetics
//			first draft of genetics framework added
//    20090304.16h40 [nicolas]: release candidate 2 -- moved to trunk
//    ...
//    (Add date, contributor name and details of modifications here as shown above)
//
// Also, check the output from 'svn log' - that's presumably a better overview of changes, actually. 

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		**WARNING: THIS CODE IS UNDER DEVELOPMENT IN A SEPERATE BRANCH**
==> if you have suggestion about modifications, please contact us directly (Nicolas, Evert)

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


=-=-=-=-=-=
DESCRIPTION
=-=-=-=-=-=

(The library is ready for use except when explicitly specified.)

====lioNeuralNet====

- perceptron with no hidden layer
- multi-layered perceptron with 1 or 2 hidden layers
- several activation function, including logistic sigmoid and hyperbolic tangeant, based on look-up tables
* note: both neural nets are implemented using arrays to speed up computation

====lioGenetics====

- a general purpose framework for genetics **under development**

====lioBehaviorLib====

- a (very) basic framework for writing your own component
- a wall-avoidance behavior using a perceptron with ad hoc weights is given as an example

====lioUtilities====

- a Fixed-point 32 integer with protected division and enables approximation instead of information loss for multiplication and division
- a random number generator based on the Hammersley quasi-random method (look-up table)
- a gaussian random number generator based on the box-muller algorithm (slow but accurate)



=-=-=-=-=-=-=-=-=
HOW TO USE LIOLIB
=-=-=-=-=-=-=-=-=

- To use this library, you need to add LioLib/liobehaviorlib, LioLib/liogenetics, LioLib/lioneuralnet, LioLib/lioutilities in your include path
	"${project_loc}/LioLib/liobehaviorlib"
	"${project_loc}/LioLib/liogenetics"
	"${project_loc}/LioLib/lioneuralnet"
	"${project_loc}/LioLib/lioutilities"  
- The liocommon/lio_global.h file contains some useful preprocessor symbol for setting the targeted system. It should be always included for tracking preprocessor symbols.
- In the current state, the library components are stable and unit test have been implemented and run off-board and on-board for most of it. These components have also been used to generate an evolving robot controller running on the cortex board.
- To use these tools in your own branch, you may simply copy the lioutilities sub-directory from the trunk to your branch and add the relevant directories to your path (ie. lioneuralnet, liogenetics, etc.). 
- We kindly ask you to send an e-mail if you are going to use these tools so that we can keep you up-to-date with addition and modification.
- The library is provided as is, please feel free to interact.


=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// nicolas and evert