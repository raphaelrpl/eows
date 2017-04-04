#ifndef __EOWS_OGC_WCS_MANAGER_HPP__
#define __EOWS_OGC_WCS_MANAGER_HPP__

// Boost dependency
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        // Forward declarations
        struct capabilities_t;
      }

      class manager : private boost::noncopyable
      {
        public:
          ~manager();

          /**
           * @brief It retrieves cached GetCapabilities meta information
           * @return Capabilities instance with values
           */
          const core::capabilities_t& capabilities() const;
          /**
           * @brief It initializes Manager scope, loading WCS meta information into memory
           */
          void initialize();

          //! Singleton style
          static manager& instance();

        private:
          //! Singleton style
          manager();
          //! Pimpl idiom
          struct impl;
          impl* pimpl_;
      };
    }
  }
}

#endif //__EOWS_OGC_WCS_MANAGER_HPP__
