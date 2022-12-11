#include "file.h"
#include <ff.h>
#include <limits>

static int              g_file_count = 0;
static std::vector<FIL> g_file_list;

int   open(const char* file, int mode) noexcept
{
      int   l_file_index = -1;
      // number of open files is smaller than the number of available vector positions: look for a free slot
      // a free slot can be identified by a NULL `obj.fs` member
      if(g_file_count < static_cast<int>(g_file_list.size())) {
          for(auto i_file = g_file_list.begin(); i_file < g_file_list.end(); i_file++) {
              auto& r_file = *i_file;
              if(r_file.obj.fs == nullptr) {
                  l_file_index = std::distance(g_file_list.begin(), i_file);
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
          FRESULT l_result = f_open(std::addressof(r_file), file, mode);
          if(l_result != FR_OK) {
              // clear the filesystem ptr such that the FIL struct can be reused by a future (and hopefuly successful) open()
              r_file.obj.fs = nullptr;
              return -1;
          }
          ++g_file_count;
      }
      return l_file_index;
}

off_t seek(int filedes, off_t offset, int whence) noexcept
{
      if((filedes >= 0) &&
          (filedes < static_cast<int>(g_file_list.size()))) {
          FIL&    r_file   = g_file_list[filedes];
          FRESULT l_result = FR_INVALID_PARAMETER;
          if(whence == SEEK_SET) {
              l_result = f_lseek(std::addressof(r_file), offset);
          } else
          if(whence == SEEK_CUR) {
              l_result = f_lseek(std::addressof(r_file), r_file.fptr + offset);
          } else
          if(whence == SEEK_END) {
              l_result = f_lseek(std::addressof(r_file), r_file.obj.objsize - offset);
          }
          if(l_result == FR_OK) {
              return r_file.fptr;
          }
      }
      return -1;
}

ssize_t read(int filedes, std::uint8_t* data, std::size_t size) noexcept
{
      if(size >= static_cast<std::size_t>(std::numeric_limits<ssize_t>::max())) {
          return -1;
      }
      if((filedes >= 0) &&
          (filedes < static_cast<int>(g_file_list.size()))) {
          FIL&     r_file   = g_file_list[filedes];
          UINT     l_read_size;
          FRESULT  l_result = f_read(std::addressof(r_file), data, size, std::addressof(l_read_size));
          if(l_result == FR_OK) {
              if(l_read_size <= std::numeric_limits<ssize_t>::max()) {
                  return static_cast<size_t>(l_read_size);
              }
          }
      }
      return -1;
}

ssize_t write(int filedes, const std::uint8_t* data, std::size_t size) noexcept
{
      if(size >= static_cast<std::size_t>(std::numeric_limits<ssize_t>::max())) {
          return -1;
      }
      if((filedes >= 0) &&
          (filedes < static_cast<int>(g_file_list.size()))) {
          FIL&     r_file   = g_file_list[filedes];
          UINT     l_write_size;
          FRESULT  l_result = f_write(std::addressof(r_file), data, size, std::addressof(l_write_size));
          if(l_result == FR_OK) {
              if(l_write_size <= std::numeric_limits<ssize_t>::max()) {
                  return static_cast<size_t>(l_write_size);
              }
          }
      }
      return -1;
}

off_t tell(int filedes) noexcept
{
      if((filedes >= 0) &&
          (filedes < static_cast<int>(g_file_list.size()))) {
          return g_file_list[filedes].fptr;
      }
      return -1;
}

bool eof(int filedes) noexcept
{
      if((filedes >= 0) &&
          (filedes < static_cast<int>(g_file_list.size()))) {
          return g_file_list[filedes].fptr == g_file_list[filedes].obj.objsize;
      }
      return false;
}

int   close(int filedes) noexcept
{
      if((filedes >= 0) &&
          filedes < static_cast<int>(g_file_list.size())) {
          FIL&    r_file   = g_file_list[filedes];
          FRESULT l_result = f_close(std::addressof(r_file));
          if(l_result != FR_OK) {
              return -1;
          }
          --g_file_count;
          return 0;
      }
      return -1;
}
