#include <functional>

namespace utils
{
/* This class used to handle system signals such as
 * Ctrl + C, Ctrl + Break and etc.
 */
class IrsSignalHandler
{
public:
  using HandlerDelegate = std::function<void()>;

  static void Initialize();

  // handler for Ctrl+C and Ctrl + Break
  static HandlerDelegate m_onTerminate;
};

}  // namespace utils
