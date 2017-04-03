#ifndef __EOWS_OGC_WCS_OPERATIONS_HPP__
#define __EOWS_OGC_WCS_OPERATIONS_HPP__

#include "../../../core/data_types.hpp"
#include "../core/data_types.hpp"

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        class operation;
      }

      namespace operations
      {
        // TODO: Remove pointer return. it is un-safe
        const eows::ogc::wcs::core::operation* build_operation(const eows::core::query_string_t&);
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_OPERATIONS_HPP__
