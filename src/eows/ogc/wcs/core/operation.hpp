#ifndef __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__
#define __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__

#include <string>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        /**
         * @brief Represents a generic WCS operation skeleton. Once defined, you must implement required methods in order to
         * determines execution behavior and even content type information
         */
        class operation
        {
          public:
            operation()
            {

            }

            virtual ~operation()
            {
            }

            /**
             * @brief It performs a WCS operation execution.
             */
            virtual void execute() = 0;

            /**
             * @brief It retrieves the content type used for this operation.
             * @return
             */
            virtual const char* content_type() const = 0;

            /**
             * @brief It retrieves the raw operation output.
             * @todo Review it. For operation like GetCoverage that processes raster information, the output is not a std string
             * @returns string representation of operation
             */
            virtual const std::string to_string() const = 0;
        };
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__
