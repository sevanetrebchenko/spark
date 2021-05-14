
#ifndef SPARK_LAYER_STACK_H
#define SPARK_LAYER_STACK_H

#include <spark/core/core.h>
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

            _NODISCARD_ std::vector<Layer*>::iterator begin();
            _NODISCARD_ std::vector<Layer*>::iterator end();
            _NODISCARD_ std::vector<Layer*>::reverse_iterator rbegin();
            _NODISCARD_ std::vector<Layer*>::reverse_iterator rend();

            _NODISCARD_ std::vector<Layer*>::const_iterator cbegin() const;
            _NODISCARD_ std::vector<Layer*>::const_iterator cend() const;
            _NODISCARD_ std::vector<Layer*>::const_reverse_iterator crbegin() const;
            _NODISCARD_ std::vector<Layer*>::const_reverse_iterator crend() const;

        private:
            class LayerStackData;
            LayerStackData* _data;
    };

}

#endif // SPARK_LAYER_STACK_H
