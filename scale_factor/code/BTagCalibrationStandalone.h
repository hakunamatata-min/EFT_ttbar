#ifndef BTagEntry_off_H
#define BTagEntry_off_H

/**
 *
 * BTagEntry_off
 *
 * Represents one pt- or discriminator-dependent calibration function.
 *
 * measurement_type:    e.g. comb, ttbar, di-mu, boosted, ...
 * sys_type:            e.g. central, plus, minus, plus_JEC, plus_JER, ...
 *
 * Everything is converted into a function, as it is easiest to store it in a
 * txt or json file.
 *
 ************************************************************/

#include <string>
#include <TF1.h>
#include <TH1.h>


class BTagEntry_off
{
public:
  enum OperatingPoint {
    OP_LOOSE=0,
    OP_MEDIUM=1,
    OP_TIGHT=2,
    OP_RESHAPING=3,
  };
  enum JetFlavor {
    FLAV_B=0,
    FLAV_C=1,
    FLAV_UDSG=2,
  };
  struct Parameters {
    OperatingPoint operatingPoint;
    std::string measurementType;
    std::string sysType;
    JetFlavor jetFlavor;
    float etaMin;
    float etaMax;
    float ptMin;
    float ptMax;
    float discrMin;
    float discrMax;

    // default constructor
    Parameters(
      OperatingPoint op=OP_TIGHT,
      std::string measurement_type="comb",
      std::string sys_type="central",
      JetFlavor jf=FLAV_B,
      float eta_min=-99999.,
      float eta_max=99999.,
      float pt_min=0.,
      float pt_max=99999.,
      float discr_min=0.,
      float discr_max=99999.
    );

  };

  BTagEntry_off() {}
  BTagEntry_off(const std::string &csvLine);
  BTagEntry_off(const std::string &func, Parameters p);
  BTagEntry_off(const TF1* func, Parameters p);
  BTagEntry_off(const TH1* histo, Parameters p);
  ~BTagEntry_off() {}
  static std::string makeCSVHeader();
  std::string makeCSVLine() const;
  static std::string trimStr(std::string str);

  // public, no getters needed
  std::string formula;
  Parameters params;

};

#endif  // BTagEntry_off_H


#ifndef BTagCalibration_off_H
#define BTagCalibration_off_H

/**
 * BTagCalibration_off
 *
 * The 'hierarchy' of stored information is this:
 * - by tagger (BTagCalibration_off)
 *   - by operating point or reshape bin
 *     - by jet parton flavor
 *       - by type of measurement
 *         - by systematic
 *           - by eta bin
 *             - as 1D-function dependent of pt or discriminant
 *
 ************************************************************/

#include <map>
#include <vector>
#include <string>
#include <istream>
#include <ostream>


class BTagCalibration_off
{
public:
  BTagCalibration_off() {}
  BTagCalibration_off(const std::string &tagger);
  BTagCalibration_off(const std::string &tagger, const std::string &filename);
  ~BTagCalibration_off() {}

  std::string tagger() const {return tagger_;}

  void addEntry(const BTagEntry_off &entry);
  const std::vector<BTagEntry_off>& getEntries(const BTagEntry_off::Parameters &par) const;

  void readCSV(std::istream &s);
  void readCSV(const std::string &s);
  void makeCSV(std::ostream &s) const;
  std::string makeCSV() const;

protected:
  static std::string token(const BTagEntry_off::Parameters &par);

  std::string tagger_;
  std::map<std::string, std::vector<BTagEntry_off> > data_;

};

#endif  // BTagCalibration_off_H


#ifndef BTagCalibration_offReader_H
#define BTagCalibration_offReader_H

/**
 * BTagCalibration_offReader
 *
 * Helper class to pull out a specific set of BTagEntry_off's out of a
 * BTagCalibration_off. TF1 functions are set up at initialization time.
 *
 ************************************************************/

#include <memory>
#include <string>



class BTagCalibration_offReader
{
public:
  class BTagCalibration_offReaderImpl;

  BTagCalibration_offReader() {}
  BTagCalibration_offReader(BTagEntry_off::OperatingPoint op,
                        const std::string & sysType="central",
                        const std::vector<std::string> & otherSysTypes={});

  void load(const BTagCalibration_off & c,
            BTagEntry_off::JetFlavor jf,
            const std::string & measurementType="comb");

  double eval(BTagEntry_off::JetFlavor jf,
              float eta,
              float pt,
              float discr=0.) const;

  double eval_auto_bounds(const std::string & sys,
                          BTagEntry_off::JetFlavor jf,
                          float eta,
                          float pt,
                          float discr=0.) const;

  std::pair<float, float> min_max_pt(BTagEntry_off::JetFlavor jf,
                                     float eta,
                                     float discr=0.) const;
protected:
  std::shared_ptr<BTagCalibration_offReaderImpl> pimpl;
};


#endif  // BTagCalibration_offReader_H

