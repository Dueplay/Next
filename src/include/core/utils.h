#ifndef NEXT_UTILS_H
#define NEXT_UTILS_H

// 删除默认拷贝和operator=
#define NON_COPYABLE(class_name) \
    class_name(const class_name&) = delete; \
    class_name operator=(const class_name&) = delete

#define NON_MOVEABLE(class_name) \
    class_name(class_name&&) = delete; \
    class_name operator=(class_name&&) = delete

#define NON_MOVE_AND_COPYABLE(class_name) \
    NON_COPYABLE(class_name); \
    NON_MOVEABLE(class_name)
#endif // !NEXT_UTILS_H

