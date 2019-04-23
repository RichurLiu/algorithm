/*
 * File: sort.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: some sort algorithms
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_SORT_HPP_
#define GLIB_SORT_HPP_
#include <cstddef> // 定义了 size_t 类型
#include <vector>
#include <assert.h>
#include <iostream>

namespace glib {
using namespace std;

//! \brief 简单实现冒泡排序、插入排序、选择排序、快速排序、归并排序、桶排序、基数排序
//!
//! \TODO
//!     1）桶排序、基数排序的基本实现，
//!     2) 目前的实现版本是从小到大排序。没有进行大到小的转换
//!     3）转换为 stl 接口形式。可以接受一个外部的谓语。比较 Key 值来排序类元素！
//!
//! \conclusion 在数值排序时，推荐使用插入排序(二分形式)。其次是不稳定的选择排序。
//!  冒泡排序、插入排序、选择排序。在排序交换数值类型时。经过测试 10000 个数据，得到的结果显示，效率如下：
//!            插入排序 > 选择排序 > 冒泡排序。倍数关系依次为 5 倍、2 倍
//!            在插入排序中，自己的实现方式：二分查找 > 后向比较 > 前向比较。依次相差 50ms、20ms
//!            在选择排序中，自己实现方式：不稳定选择排序 > 稳定选择排序(类似冒泡)。约为 2 倍的关系

// 选择某个排序方法
enum class SortOption {
    kBubble,
    kInsertion,
    kSelection,
    kMerge,
    kQuick
};

// 打印当前数组元素顺序值
template <typename _Scalar>
void SortDebug(const vector<_Scalar> &array) {
    for (auto &b: array)
        cout << " " << b;
    cout << endl;
}

// 排序算法详细实现---冒泡排序、插入排序、选择排序、归并排序、快速排序
class SortDetail {

template <typename _Scalar>
friend void Sort(vector<_Scalar> &array, const SortOption option = SortOption::kBubble);

//---------------------------冒泡排序-------------------------------------------------

template <typename _Scalar>
void Bubble(vector<_Scalar> &array) {
    cout << "Bubble Sort" << endl;
    assert(array.size() != 0);
    auto n = array.size();
    for (size_t i = 0; i < n; i++) {
        bool success_sort = false;
        for (size_t j = 0; j < n - 1 - i; j++) {
            if (array[j] > array[j+1]) {
                // 满足交换顺序，可以直接交换
                _Scalar temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;

                success_sort = true;
            }
        }
        if (!success_sort) return; // 数据已经是有序状态
    }
}


//----------------------------插入排序------------------------------------------------


// 插入排序:移动操作总数等于逆序度。不管什么操作，移动次数是不变的
// 可以分别注释下面的三种遍历方式 1)二分查找方式 2）前向遍历 3）后向遍历 看看不同的效果对应的时间
// 经过测试 二分查找方式执行时间最短。是因为交换移动次数是固定的，只要比较的次数少一些。时间就会少！
template <typename _Scalar>
void Insertion(vector<_Scalar> &array) {
    cout << "Insertion Sort" << endl;
    assert(array.size() != 0);
    auto n = array.size();
    if (1 == n) return;

    // 查找插入方法用的是从头到尾的方法
    for (size_t separate_point = 1; separate_point < n; ++separate_point) {
        auto separate_point_value = array[separate_point];
        //------分割线---------------
        // 1)利用二分查找方式:从前到后 。这种方式的运行效率最高。比下面的尾到头的方式时间要短了 50 ms
        size_t low_limit = 0;
        size_t up_limit = separate_point - 1;
        // size_t middle = (low_limit + up_limit)/2;
        size_t middle = low_limit + (up_limit - low_limit)/2;
        while (low_limit <= up_limit) { // 二分查找
            if ((0 == middle && array[middle] > separate_point_value) || // 保证稳定排序，只有大于当前 separate_point 对应的值才会交换
                (middle >= 1 &&
                array[middle] > separate_point_value &&
                array[middle-1] <= separate_point_value)) {
                // 插入第一个位置或者中间某个位置，交换
                auto temp_value = array[separate_point];
                for (size_t j = separate_point; j > middle; --j)
                    array[j] = array[j-1];
                array[middle] = temp_value;
                break;
            }

            if (array[middle] <= separate_point_value) { // 保证稳定排序
                low_limit = middle + 1;
            } else {
                up_limit = middle - 1;
            }
            middle = low_limit + (up_limit - low_limit)/2;
            // middle = (low_limit + up_limit)/2;
        }

        // 2)遍历循环查找：从头到尾。实现的方式效率最低。
        // for (size_t i = 0; i < separate_point; ++i) { // 遍历查找复杂度高，因为假定前面已经是有序的了，所以可以使用二分查找
        //     if (separate_point_value < array[i]) {
        //         // 交换位置
        //         auto temp_value = array[separate_point];
        //         for (size_t j = separate_point; j > i ; --j)
        //             array[j] = array[j-1];
        //         array[i] = temp_value;
        //         break; // 交换完成后直接退出当前循环
        //     }
        // }

        // 3)遍历循环查找：从尾到头，一遍比较一遍交换。当前实现的效率不是最高的。比上面第 2 中方式高些。1000 个数句差了 20 ms 左右
        // int i = separate_point - 1;
        // for (; i >= 0 ; --i) {
        //     if (array[i] > separate_point_value) // 向后移动一位
        //         array[i+1] = array[i];
        //     else
        //         break;
        // }
        // array[i+1] = separate_point_value;

        // debug
        // 每次排序完成，打印一下顺序
        // cout << "separate_point: " << separate_point << " and its value: " << separate_point_value << endl;
        // SortDebug(array);
    }
}


//----------------------------选择排序------------------------------------------------


// 选择排序（稳定版）：从剩下的未排序数组中找到最小值，然后放到已排序序列的后面。
// 下面的实现方式，本质上与冒泡排序一致。是一个稳定的选择排序
template <typename _Scalar>
void SelectionStable(vector<_Scalar> &array) {
    assert(array.size() != 0);
    cout << "Selection Sort" << endl;

    // 需要遍历 n 次，每次寻找一个最小值。可以从后向前遍历。一边遍历一边交换顺序，实际上是上面插入排序以及冒泡排序实现的思路。
    for (size_t i = 0; i < array.size(); ++i) {
        int j = array.size() - 1;
        bool success_sort = false;
        for (; j > i; --j) { // 在剩下的未排序数组中找到最小值
            if (array[j] < array[j-1]) {
                auto temp_value = array[j-1];
                array[j-1] = array[j];
                array[j] = temp_value;
                success_sort = true;
            }
        }
        if (!success_sort) return;
    }
}

// 不稳定的选择排序，其时间要比上面的稳定选择排序快。仅仅针对数值内置类型。
template <typename _Scalar>
void SelectionUnstable(vector<_Scalar> &array) {
    assert(array.size() != 0);
    cout << "Unstable Selection" << endl;
    for (size_t i = 0; i < array.size() - 1; ++i) {
        auto min_index = i;
        for (size_t j = i + 1; j < array.size(); ++j) {
            if (array[j] < array[min_index])
                min_index = j;
        }
        // 交换元素
        if (min_index != i) {
            auto temp_value = array[min_index];
            array[min_index] = array[i];
            array[i] = temp_value;
        }
    }
}


//------------------------------归并排序----------------------------------------------


// 归并排序(Merge Sort)。要比上面的插入排序快了 10 倍左右

//! \brief 将两个有序数组合并为一个有序数组。利用选择排序的思路 + 二分查找方法进行。默认从 2 向 1 插入。
//! \complexity: O(nlog(n))
// template <typename _Scalar>
// vector<_Scalar> Merge(const vector<_Scalar> &ordered_array1, const vector<_Scalar> &ordered_array2) {
//     vector<_Scalar> merge_array(ordered_array1);
//     merge_array.reserve(ordered_array1.size() + ordered_array2.size()); // 调整数组大小
//
//     for (size_t i = 0; i < ordered_array2.size(); i++) {
//         auto separate_point_value = ordered_array2[i]; // 插入数组第 i 个值
//         // cout << "merge_array.size() : " << merge_array.size();
//         // cout << " separate_point: " << separate_point_value << endl;
//         size_t low_limit = 0;
//         size_t up_limit = merge_array.size() - 1; // 仅仅查询数组 1 那么大的范围
//         size_t middle = low_limit + (up_limit - low_limit)/2;
//         while (low_limit <= up_limit) { // 二分查找
//             if ((0 == middle && merge_array[middle] > separate_point_value) || // 保证稳定排序，只有大于当前 separate_point 对应的值才会交换
//                 (middle >= 1 &&
//                 merge_array[middle] > separate_point_value &&
//                 merge_array[middle-1] <= separate_point_value)) {
//                 // 插入第一个位置或者中间某个位置，交换
//                 merge_array.push_back(ordered_array2[i]);
//                 auto temp_value = *(merge_array.end()-1); // 获取最后一个值
//                 for (size_t j = merge_array.size() - 1; j > middle; --j)
//                     merge_array[j] = merge_array[j-1];
//                 merge_array[middle] = temp_value;
//                 break;
//             }
//
//             if (merge_array[middle] <= separate_point_value) { // 保证稳定排序
//                 low_limit = middle + 1;
//             } else {
//                 up_limit = middle - 1;
//             }
//             middle = low_limit + (up_limit - low_limit)/2;
//         }
//         if (low_limit > up_limit)
//             merge_array.push_back(ordered_array2[i]);
//     }
//     return merge_array;
// }

// 比较两个有序数组的最低位元素。哪个小就复制哪个元素到 merge_array 中。然后移动到下一个元素在与其比较。
//! \note 这种合并的方式要比上面的合并方式快 10 倍。 O(n)
template <typename _Scalar>
vector<_Scalar> Merge(const vector<_Scalar> &ordered_array1,const vector<_Scalar> &ordered_array2) {
    vector<_Scalar> merge_array(ordered_array1.size() + ordered_array2.size());
    auto iter1 = ordered_array1.begin();
    auto iter2 = ordered_array2.begin();
    auto iter = merge_array.begin();

    for (; iter != merge_array.end(); ++iter) {
        if (iter1 != ordered_array1.end() && iter2 != ordered_array2.end()) {
            if (*iter1 > *iter2) {
                *iter = *iter2;
                ++iter2;
            }
            else {
                *iter = *iter1;
                ++iter1;
            }
        } else {
            if (iter1 == ordered_array1.end()) {
                *iter = *iter2;
                ++iter2;
            } else {
                *iter = *iter1;
                ++iter1;
            }
        }
    }
    return merge_array;
}

// 利用引用方式归并排序 O(nlog(n))
template <typename _Scalar>
void MergeSort(vector<_Scalar> &array) {
    if (1 == array.size())
        return ;
    vector<_Scalar> array1(array.begin(), array.begin() + array.size() / 2); // 不包含最后一位
    vector<_Scalar> array2(array.begin() + array.size() / 2, array.end());
    MergeSort(array1);
    MergeSort(array2);
    array = Merge(array1, array2); // 这里可以改为将 array 写入到 Merge() 形参里面，
}

// 利用 vector 拷贝的引用方式。比较直观！
// template <typename _Scalar>
// vector<_Scalar>  MergeSortBase(vector<_Scalar> array) {
//     if (1 == array.size())
//         return array;
//     vector<_Scalar> array1(array.begin(), array.begin() + array.size() / 2); // 不包含最后一位
//     vector<_Scalar> array2(array.begin() + array.size() / 2, array.end());
//     return Merge(MergeSortBase(array1), MergeSortBase(array2));
// }
//
// template <typename _Scalar>
// void MergeSort(vector<_Scalar> &array) {
//     cout << "MergeSort" << endl;
//     array = MergeSortBase(array);
// }


//-----------------------------快速排序-----------------------------------------------


// 分区函数
//! \param temp_start 下次分界点起始
//! \param temp_end 下次分界终点
//! \note 需要额外的空间一般是 O(n)
// template <typename _Scalar>
// void Partition(typename vector<_Scalar>::iterator begin, typename vector<_Scalar>::iterator end,
//                typename vector<_Scalar>::iterator &temp_start, typename vector<_Scalar>::iterator &temp_end) {
//     const typename vector<_Scalar>::iterator pivot = begin + (end - begin)/2; // 默认选择中间的元素作为 pivot
//     vector<_Scalar> left;
//     vector<_Scalar> right;
//     size_t equal_num = 0;
//
//     // 将小于 pivot 元素放到依次从左边放，大于 pivot 的元素依次在右边放
//     auto start = begin;
//     for (; start != end; ++start) {
//         if (*start < *pivot) {
//             left.push_back(*start);
//         } else if (*start > *pivot){
//             right.push_back(*start);
//         } else {
//             // 相等的元素
//             equal_num++;
//         }
//     }
//
//     for (size_t i = 0; i < equal_num; ++i) // 复制相等元素
//         left.push_back(*pivot);
//
//     start = begin;
//     decltype(start) start_left = left.begin();
//     decltype(start) start_right = right.begin();
//
//     // 拷贝左元素
//     for (; start_left != left.end(); ++start_left, ++start)
//         *start = *start_left;
//
//     // 保存下一次的分界点
//     temp_start = start;
//     temp_end = start - equal_num; // 保留左边界结尾，把重合的元素不作为下步的排序。会加速程序 2ms 左右
//
//     // 拷贝右边元素
//     for (; start_right != right.end(); ++start_right, ++start)
//         *start = *start_right;
// }

// 此时的分区函数速度最快，是上面分区函数的 10 倍。空间复杂度为 O(1)
template <typename _Scalar>
void Partition(typename vector<_Scalar>::iterator begin,
               typename vector<_Scalar>::iterator end,
               typename vector<_Scalar>::iterator &temp_start,
               typename vector<_Scalar>::iterator &temp_end) {
    auto pivot = end-1; // 以数组的最后一个值为分界点
    temp_start = begin; // 仅仅把这两个值当做循环变量来用。
    temp_end = begin;
    for (; temp_end != end; ++temp_end) {
        if (*temp_end < *pivot) {
            auto temp = *temp_start;
            *temp_start = *temp_end;
            *temp_end = temp;
            ++temp_start;
        }
    }
    // 交换最后一个 pivot 值
    auto temp = *pivot;
    *pivot = *temp_start;
    *temp_start = temp;

    // 把下一次分区的起始和结尾更新
    temp_end = temp_start;
    temp_start = temp_end + 1;
}

// 快速排序递归函数，这里的 end 数据是不使用的
template <typename _Scalar>
void QuickSortBase(typename vector<_Scalar>::iterator begin,
                   typename vector<_Scalar>::iterator end) { // note 这里忘了加 typename 了引起了歧义
    // 选择中间的元素
    if (begin == end)
        return;
    decltype(begin) temp_end; // 左边界尾部
    decltype(begin) temp_start; // 右边界起点
    Partition<_Scalar>(begin, end, temp_start, temp_end);
    QuickSortBase<_Scalar>(begin, temp_end);
    QuickSortBase<_Scalar>(temp_start, end);
}
template <typename _Scalar>
void QuickSort(vector<_Scalar> &array) {
    cout << "Quick Sort" << endl;
    QuickSortBase<_Scalar>(array.begin(), array.end());
}

}; // class SortDetail

// 上述排序算法/思想简单应用
class AppDetail {
template <typename _Scalar>
friend _Scalar FindKthBigElement(vector<_Scalar> array, int kth);

//--------------------快排思路{分治+分区}查找第 K 大元素----------------------------------

// 分区函数，第 k 大元素，核心函数
template <typename _Scalar>
void FindPartition(typename vector<_Scalar>::iterator begin,
                   typename vector<_Scalar>::iterator end,
                   typename vector<_Scalar>::iterator &temp_start,
                   typename vector<_Scalar>::iterator &temp_end) {
    auto pivot = end-1; // 以数组的最后一个值为分界点
    temp_start = begin; // 仅仅把这两个值当做循环变量来用。
    temp_end = begin;
    for (; temp_end != end; ++temp_end) {
       if (*temp_end > *pivot) {
           auto temp = *temp_start;
           *temp_start = *temp_end;
           *temp_end = temp;
           ++temp_start;
       }
    }
    // 交换最后一个 pivot 值
    auto temp = *pivot;
    *pivot = *temp_start;
    *temp_start = temp;

    // 把下一次分区的起始和结尾更新
    temp_end = temp_start; // 就是 pivot 点
    temp_start = temp_end + 1;
}

// 第 k 大元素，核心函数
template <typename _Scalar>
_Scalar QuickFind(typename vector<_Scalar>::iterator begin,
                  typename vector<_Scalar>::iterator end, int kth) {
    decltype(begin) temp_end; // 左边界尾部
    decltype(begin) temp_start; // 右边界起点
    FindPartition<_Scalar>(begin, end, temp_start, temp_end); // 左边是大的元素，右边是小的元素

    auto pivot = temp_end - begin; // 分界点位置索引
    if (pivot  == kth - 1)
        return *temp_end;
    else if (pivot > kth - 1) // 在左边找
        return QuickFind<_Scalar>(begin, temp_end, kth);
    else {
        kth = kth - 1 - pivot;
        return QuickFind<_Scalar>(temp_start, end, kth);
    }
}


//-----------------------------------------------------------------------------------

}; // class AppDetail


//--------------------------------------外部功能接口--------------------------------------


//! \brief 各种排序算法接口
//! \note 调用是需要给定一个 vector 实例。不能是简单临时构造
//! \param array 输入的数据元素，只支持 vector 容器
//! \complexity 冒泡：O(n^2)       插入: O(n^2)      选择：O(n^2)
//!             归并：O(nlog(n))   快排：O(nlog(n))
template <typename _Scalar>
void Sort(vector<_Scalar> &array, const SortOption option = SortOption::kBubble) {
    SortDetail sort;
    switch (option) {
        case SortOption::kBubble: {
            sort.Bubble(array);
            break;
        }
        case SortOption::kInsertion: {
            sort.Insertion(array);
            break;
        }
        case SortOption::kSelection: {
            // SelectionStable(array); // 稳定版本选择排序
            sort.SelectionUnstable(array); // 不稳定排序算法，对于排序和交换的原始是内置数值类型，
                                              // 此时不稳定的排序要快于稳定的排序。
            break;
        }
        case SortOption::kMerge: {
            cout << "Merge Sort" << endl;
            sort.MergeSort(array);
            break;
        }
        case SortOption::kQuick: {
            sort.QuickSort(array);
            break;
        }
    }
}

//! \brief 在无序数组中找到第 K 大元素
//! \note 对于重复的元素也会算进去。如果想找不包含重复的元素中的第 K 大元素，
//!       那么需要自己实现把重复的元素剔除掉，然后在调用该函数即可找到第 k 大元素
//! \param array 输入的无序 vector
//! \return 第 k 大元素对应的值
//! \complexity O(n)
template <typename _Scalar>
_Scalar FindKthBigElement(vector<_Scalar> array, int kth) {
    AppDetail findkth;
    return findkth.QuickFind<_Scalar>(array.begin(), array.end(), kth);
}

} // namespace glib
#endif