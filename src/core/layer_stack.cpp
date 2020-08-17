
#include <core/layer_stack.h>
#include <spark_pch.h>

namespace Spark {
    //------------------------------------------------------------------------------------------------------------------
    // LAYER STACK DATA
    //------------------------------------------------------------------------------------------------------------------
    class LayerStack::LayerStackData {
        public:
            LayerStackData();
            ~LayerStackData();

            void PushOverlay(Layer* overlay);
            void PopOverlay(Layer* overlay);

            void PushLayer(Layer* layer);
            void PopLayer(Layer* layer);

        private:
            // Layers first, overlays last
            // Ordered in reverse draw order (overlays get drawn on top).
            std::vector<Layer*> _layerStack;
            unsigned _layerInsertIndex;
    };

    LayerStack::LayerStackData::LayerStackData() : _layerInsertIndex(0) {
        // Nothing to do here.
    }

    LayerStack::LayerStackData::~LayerStackData() {
        for (Layer* layer : _layerStack) {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::LayerStackData::PushOverlay(Layer *overlay) {
        // Overlays get pushed to the end of the layer stack.
        _layerStack.emplace_back(overlay);
    }

    void LayerStack::LayerStackData::PopOverlay(Layer *overlay) {
        // Overlays start at the layer insert index and end at the end of the vector.
        auto layerIterator = std::find(_layerStack.begin() + _layerInsertIndex, _layerStack.end(), overlay);
        if (layerIterator != _layerStack.end()) {
            overlay->OnDetach();
            _layerStack.erase(layerIterator);
        }
    }

    void LayerStack::LayerStackData::PushLayer(Layer *layer) {
        // Emplace at the end of the layer stack position.
        _layerStack.emplace(_layerStack.begin() + _layerInsertIndex, layer);
        ++_layerInsertIndex;
    }

    void LayerStack::LayerStackData::PopLayer(Layer *layer) {
        // Layers start at index 0 and go until the layer insert index.
        auto endLayerIterator = _layerStack.begin() + _layerInsertIndex;
        auto layerIterator = std::find(_layerStack.begin(), endLayerIterator, layer);
        // Found layer.
        if (layerIterator != endLayerIterator) {
            layer->OnDetach();
            _layerStack.erase(layerIterator);
            --_layerInsertIndex;
        }
    }

    //------------------------------------------------------------------------------------------------------------------
    // LAYER STACK
    //------------------------------------------------------------------------------------------------------------------
    LayerStack::LayerStack() : _data(new LayerStackData()){
        // Nothing to do here.
    }

    void LayerStack::PushOverlay(Layer *overlay) {
        _data->PushOverlay(overlay);
    }

    void LayerStack::PopOverlay(Layer *overlay) {
        _data->PopOverlay(overlay);
    }

    void LayerStack::PushLayer(Layer *layer) {
        _data->PushLayer(layer);
    }

    void LayerStack::PopLayer(Layer *layer) {
        _data->PopLayer(layer);
    }

    LayerStack::~LayerStack() {
        delete _data;
    }


}
