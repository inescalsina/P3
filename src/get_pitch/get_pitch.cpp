/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>
#include <vector>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Estimator 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -m FLOAT, --umaxnorm=FLOAT  umbral de la autocorrelación a largo plazo [default: 0.38]
    -s FLOAT, --ucc=FLOAT  Umbral de recorte para el center-clipping [default: 0.0045]
    -h, --help  Show this screen
    --version   Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the estimation:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
  /// \HECHO 
  ///  Argumento umaxnorm (umbral para la autocorrelacion a largo plazo)
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float umaxnorm = stof(args["--umaxnorm"].asString());
  float ucc = stof(args["--ucc"].asString());

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, umaxnorm, PitchAnalyzer::HAMMING, 50, 500);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  /// \HECHO
  /// Hemos progamdo central-clipping
  float umbral_cc = ucc;
  for (unsigned int i = 0; i < x.size(); i++){
    if (abs(x[i]) < umbral_cc){
      x[i] = 0;
    }
    if (x[i] > umbral_cc){
      x[i] = x[i] - umbral_cc;
    }
    if (x[i] < - umbral_cc){
      x[i] = x[i] + umbral_cc;
    }
  }
  
  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }



  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  /// \HECHO
  /// Hemos implementado un filtro de mediana de orden 3
  vector<float> f0m(f0.size());
  f0m = f0;
  for (unsigned int j = 1; j < f0.size() - 1; j++){
    vector<float> vect = {f0[j-1], f0[j], f0[j+1]};
    sort (vect.begin(), vect.end());
    f0m[j] = vect[1];
  }
  f0 = f0m;

  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
