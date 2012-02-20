#include "renderWindow.hpp"

namespace blub
{
namespace gui
{


void renderWindow::saveScreenshot(string prefix, string suffix)
{
    m_rw->writeContentsToTimestampedFile(prefix, suffix);
}


}
}
