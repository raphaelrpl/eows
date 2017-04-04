#ifndef __EOWS_OGC_WCS_OPERATIONS_GET_CAPABILITIES_HPP__
#define __EOWS_OGC_WCS_OPERATIONS_GET_CAPABILITIES_HPP__

#include "../core/operation.hpp"

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace operations
      {
        class get_capabilities : public eows::ogc::wcs::core::operation
        {
          public:
            get_capabilities();
            ~get_capabilities();

            void execute();

            /**
             * @brief For WCS GetCapabilities operations, it is fixed to "application/gml+xml"
             * @return
             */
            const char* content_type() const;

            const std::string to_string() const override;
          private:
        };
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_OPERATIONS_GET_CAPABILITIES_HPP__
