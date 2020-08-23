
#include <spark_pch.h>        // std::vector
#include <core/layer_stack.h> // LayerStack

namespace Spark {
    //------------------------------------------------------------------------------------------------------------------
    // LAYER STACK DATA
    //------------------------------------------------------------------------------------------------------------------
    class LayerStack::LayerStackData {
        public:
            LayerStackData();
            ~LayerStackData();

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

    //------------------------------------------------------------------------------------------------------------------
    // LAYER STACK
    //------------------------------------------------------------------------------------------------------------------
    LayerStack::LayerStack() : _data(new LayerStackData()){
        // Nothing to do here.
    }

    LayerStack::~LayerStack() {
        delete _data;
    }

    void LayerStack::PushOverlay(Layer *overlay) {
        // Overlays get pushed to the end of the layer stack.
        _data->_layerStack.emplace_back(overlay);
    }

    void LayerStack::PopOverlay(Layer *overlay) {
        // Overlays start at the layer insert index and end at the end of the vector.
        auto layerIterator = std::find(_data->_layerStack.begin() + _data->_layerInsertIndex, _data->_layerStack.end(), overlay);
        if (layerIterator != _data->_layerStack.end()) {
            overlay->OnDetach();
            _data->_layerStack.erase(layerIterator);
        }
    }

    void LayerStack::PushLayer(Layer *layer) {
        // Emplace at the end of the layer stack position.
        _data->_layerStack.emplace(_data->_layerStack.begin() + _data->_layerInsertIndex, layer);
        ++_data->_layerInsertIndex;
    }

    void LayerStack::PopLayer(Layer *layer) {
        // Layers start at index 0 and go until the layer insert index.
        auto endLayerIterator = _data->_layerStack.begin() + _data->_layerInsertIndex;
        auto layerIterator = std::find(_data->_layerStack.begin(), endLayerIterator, layer);
        // Found layer.
        if (layerIterator != endLayerIterator) {
            layer->OnDetach();
            _data->_layerStack.erase(layerIterator);
            --_data->_layerInsertIndex;
        }
    }

    std::vector<Layer*>::iterator LayerStack::begin() {
        return _data->_layerStack.begin();
    }

    std::vector<Layer*>::iterator LayerStack::end() {
        return _data->_layerStack.end();
    }

    std::vector<Layer*>::reverse_iterator LayerStack::rbegin() {
        return _data->_layerStack.rbegin();
    }

    std::vector<Layer*>::reverse_iterator LayerStack::rend() {
        return _data->_layerStack.rend();
    }

    std::vector<Layer*>::const_iterator LayerStack::cbegin() const {
        return _data->_layerStack.cbegin();
    }

    std::vector<Layer*>::const_iterator LayerStack::cend() const {
        return _data->_layerStack.cend();
    }

    std::vector<Layer*>::const_reverse_iterator LayerStack::crbegin() const {
        return _data->_layerStack.crbegin();
    }

    std::vector<Layer*>::const_reverse_iterator LayerStack::crend() const {
        return _data->_layerStack.crend();
    }

}
