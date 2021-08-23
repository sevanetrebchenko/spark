//
//
//namespace Spark::Memory {
//
//    Handle::Handle() : data_(nullptr),
//                       inUse_(false)
//                       {
//    }
//
//    Handle::~Handle() {
//        data_ = nullptr;
//        inUse_ = false;
//    }
//
//    void Handle::Reset() {
//        data_ = nullptr;
//        inUse_ = false;
//    }
//
//    void Handle::Reset(void* data) {
//        data_ = data;
//        inUse_ = true;
//    }
//
//    void* Handle::GetData() const {
//        return data_;
//    }
//
//    bool Handle::InUse() const {
//        return inUse_;
//    }
//
//}
