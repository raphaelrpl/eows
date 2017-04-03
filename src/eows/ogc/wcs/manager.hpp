#ifndef __EOWS_OGC_WCS_MANAGER_HPP__
#define __EOWS_OGC_WCS_MANAGER_HPP__

#include <boost/noncopyable.hpp>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        struct capabilities_t;
      }

      class manager : private boost::noncopyable
      {
        public:
          const core::capabilities_t& capabilities() const;

          void initialize();

          static manager& instance();

        private:
          // Singleton style
          manager();

          struct impl;
          impl* pimpl_;
      };
    }
  }
}

#endif //__EOWS_OGC_WCS_MANAGER_HPP__
