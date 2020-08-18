
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

            _NODISCARD_ std::vector<Layer*>::iterator begin();
            _NODISCARD_ std::vector<Layer*>::iterator end();
            _NODISCARD_ std::vector<Layer*>::reverse_iterator rbegin();
            _NODISCARD_ std::vector<Layer*>::reverse_iterator rend();

            _NODISCARD_ std::vector<Layer*>::const_iterator cbegin() const;
            _NODISCARD_ std::vector<Layer*>::const_iterator cend() const;
            _NODISCARD_ std::vector<Layer*>::const_reverse_iterator crbegin() const;
            _NODISCARD_ std::vector<Layer*>::const_reverse_iterator crend() const;

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

    std::vector<Layer *>::iterator LayerStack::LayerStackData::begin() {
        return _layerStack.begin();
    }

    std::vector<Layer *>::iterator LayerStack::LayerStackData::end() {
        return _layerStack.end();
    }

    std::vector<Layer *>::reverse_iterator LayerStack::LayerStackData::rbegin() {
        return _layerStack.rbegin();
    }

    std::vector<Layer *>::reverse_iterator LayerStack::LayerStackData::rend() {
        return _layerStack.rend();
    }

    std::vector<Layer *>::const_iterator LayerStack::LayerStackData::cbegin() const {
        return _layerStack.cbegin();
    }

    std::vector<Layer *>::const_iterator LayerStack::LayerStackData::cend() const {
        return _layerStack.cend();
    }

    std::vector<Layer *>::const_reverse_iterator LayerStack::LayerStackData::crbegin() const {
        return _layerStack.crbegin();
    }

    std::vector<Layer *>::const_reverse_iterator LayerStack::LayerStackData::crend() const {
        return _layerStack.crend();
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

    std::vector<Layer *>::iterator LayerStack::begin() {
        return _data->begin();
    }

    std::vector<Layer *>::iterator LayerStack::end() {
        return _data->end();
    }

    std::vector<Layer *>::reverse_iterator LayerStack::rbegin() {
        return _data->rbegin();
    }

    std::vector<Layer *>::reverse_iterator LayerStack::rend() {
        return _data->rend();
    }

    std::vector<Layer *>::const_iterator LayerStack::cbegin() const {
        return _data->cbegin();
    }

    std::vector<Layer *>::const_iterator LayerStack::cend() const {
        return _data->cend();
    }

    std::vector<Layer *>::const_reverse_iterator LayerStack::crbegin() const {
        return _data->crbegin();
    }

    std::vector<Layer *>::const_reverse_iterator LayerStack::crend() const {
        return _data->crend();
    }


}
