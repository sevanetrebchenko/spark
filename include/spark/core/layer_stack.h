
#ifndef SPARK_LAYER_STACK_H
#define SPARK_LAYER_STACK_H

#include <spark/core/rename.h>
#include <spark/core/layer.h> // Layer

namespace Spark {

    class LayerStack {
        public:
            LayerStack();
            ~LayerStack();

            void PushOverlay(Layer* overlay);
            void PopOverlay(Layer* overlay);

            void PushLayer(Layer* layer);
            void PopLayer(Layer* layer);

            NODISCARD std::vector<Layer*>::iterator begin();
            NODISCARD std::vector<Layer*>::iterator end();
            NODISCARD std::vector<Layer*>::reverse_iterator rbegin();
            NODISCARD std::vector<Layer*>::reverse_iterator rend();

            NODISCARD std::vector<Layer*>::const_iterator cbegin() const;
            NODISCARD std::vector<Layer*>::const_iterator cend() const;
            NODISCARD std::vector<Layer*>::const_reverse_iterator crbegin() const;
            NODISCARD std::vector<Layer*>::const_reverse_iterator crend() const;

        private:
            class LayerStackData;
            LayerStackData* _data;
    };

}

#endif // SPARK_LAYER_STACK_H
