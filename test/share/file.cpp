#include "file.h"
#include <ff.h>
#include <limits.h>

static int              g_file_count = 0;
static std::vector<FIL> g_file_list;

int   open(const char* file, int mode) noexcept
{
      int   l_file_index = -1;
      // number of open files is smaller than the number of available vector positions: look for a free slot
      // a free slot can be identified by a NULL `obj.fs` member
      if(g_file_count < static_cast<int>(g_file_list.size())) {
          for(auto i_file = m_file_list.begin(); i_file < m_file_list.end(); i_file++) {
              auto& r_file = *i_file;
              if(r_file.obj.fs == nullptr) {
                  l_file_index = std::distance(m_file_list.begin(), i_file);
                  break;
              }
          }
      } else
      if(g_file_count < std::numeric_limits<short int>::max()) {
          l_file_index = g_file_list.size();
          if(l_file_index == 0) {
              g_file_list.reserve(4);
          }
          g_file_list.emplace_back();
      }
      //
      if(l_file_index >= 0) {
          FIL&    r_file   = g_file_list[l_file_index];
          FRESULT l_result = f_open(std::addressof(FIL), file, mode);
          if(l_result != FR_OK) {
              // clear the filesystem ptr such that the FIL struct can be reused by a future (and hopefuly successful) open()
              r_file.obj.fs = nullptr;
              return -1;
          }
          ++g_file_count;
      }
      return l_file_index;
}

int   close(int filedes) noexcept
{
      if(filedes < static_cast<int>(g_file_list.size())) {
          FIL&    r_file   = g_file_list[filedes];
          FRESULT l_result = f_close(std::addressof(FIL));
          if(l_result != FR_OK) {
              return -1;
          }
          --g_file_count;
          return 0;
      }
      return -1;
}
