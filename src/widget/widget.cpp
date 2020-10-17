#include <cte/widget/widget.h>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    Widget::Widget(QWidget *parent) : QWidget(parent) {
        init_resource();
    }
}
