#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

// STL
#include <string>
#include <vector>
#include <memory>

//! Define the EOWS Context.
namespace eows
{
  //! Define the WTSCS Context.
  namespace wtscs
  {

    struct coordinate
    {
      double x, y;
      coordinate(double paramx, double paramy) : x(paramx), y(paramy) {}
    };

    /*! \brief Generic input parameters structure.
         *
         *
         */
    struct base_input_parameters
    {
      virtual ~base_input_parameters() = default;
    };

    /*! \brief Input parameters structure for TWDTW algorithm.
         *
         *
         */
    struct twdtw_input_parameters : base_input_parameters
    {
      std::string cv_name;
      std::vector<std::string> attributes;
      std::vector<coordinate> roi;
      std::string start_date;
      std::string end_date;
      std::string by;
      double overlap;
      double alpha;
      double beta;
      std::string output_array_name;
    };

    /*! \brief Binomial structure algorithm-input parameters.
         *
         *
         */
    class parseRequest
    {
      std::string algorithm;
      std::unique_ptr<base_input_parameters> input_parameters;
    public:
      /*! \brief Parse Request.
       *
       *  The request is parsed into Document Object Model (DOM).
       */
      void setParameters(const char *);
    };

  } // end namespace wtscs
} // end namespace eows

#endif // PARSEREQUEST_HPP
