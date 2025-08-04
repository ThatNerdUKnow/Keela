//
// Created by brand on 5/31/2025.
//

#ifndef GTKSINK_H
#define GTKSINK_H
#include "simpleelement.h"
#include "widgetelement.h"

namespace Keela {
    class GtkSink final : public SimpleElement, public WidgetElement {
    public:
        GtkSink();
        explicit GtkSink(const std::string &name);
        ~GtkSink() override;

        gpointer get_widget() override;
    };
}
#endif //GTKSINK_H
