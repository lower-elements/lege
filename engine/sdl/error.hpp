#ifndef LIBLEGE_sdl_ERROR_HPP
#define LIBLEGE_sdl_ERROR_HPP

#include <exception>
#include <string>

namespace lege::sdl {

class Error : public std::exception {
public:
  Error(std::string prefix = "sdl error");
  virtual ~Error() override;

  virtual const char *what() const noexcept override;

private:
  std::string m_msg;
};

} // namespace lege::sdl

#endif
