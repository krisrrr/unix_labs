import time
from random import randint
from multiprocessing import Process

LEN_OF_LIST = 100000

def make_list():
    maked_list = [randint(1, LEN_OF_LIST) for i in range(LEN_OF_LIST)]
    return maked_list

############################## QUICK SORT #################################
def partition(array, begin, end):
    pivot_idx = begin
    for i in range(begin+1, end+1):
        if array[i] <= array[begin]:
            pivot_idx += 1
            array[i], array[pivot_idx] = array[pivot_idx], array[i]
    array[pivot_idx], array[begin] = array[begin], array[pivot_idx]
    return pivot_idx


def quick_sort_recursion(array, begin, end):
    if begin >= end:
        return
    pivot_idx = partition(array, begin, end)
    quick_sort_recursion(array, begin, pivot_idx-1)
    quick_sort_recursion(array, pivot_idx+1, end)


def quick_sort(array, begin=0, end=None):              
    if end is None:
        end = len(array) - 1
    return quick_sort_recursion(array, begin, end)


def Q_S(array):
    print('Sorting by Quick sort started')
    time_of_start = time.time() 
    quick_sort(array)
    wasted_time = time.time() - time_of_start
    print(f'Sorting by Quick sort finished. {wasted_time} seconds were spent')
    #return array



######################## Merge Sort ##################################
def merge_sort(arr):
    # Последнее разделение массива
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    # Выполняем merge_sort рекурсивно с двух сторон
    left, right = merge_sort(arr[:mid]), merge_sort(arr[mid:])

    # Объединяем стороны вместе
    return merge(left, right, arr.copy())


def merge(left, right, merged):

    left_cursor, right_cursor = 0, 0
    while left_cursor < len(left) and right_cursor < len(right):
      
        # Сортируем каждый и помещаем в результат
        if left[left_cursor] <= right[right_cursor]:
            merged[left_cursor+right_cursor]=left[left_cursor]
            left_cursor += 1
        else:
            merged[left_cursor + right_cursor] = right[right_cursor]
            right_cursor += 1
            
    for left_cursor in range(left_cursor, len(left)):
        merged[left_cursor + right_cursor] = left[left_cursor]
        
    for right_cursor in range(right_cursor, len(right)):
        merged[left_cursor + right_cursor] = right[right_cursor]

    return merged


def M_S(arr):
    print('Sorting by Merge sort started')
    time_of_start = time.time() 
    merge_sort(arr)
    wasted_time = time.time() - time_of_start
    print(f'Sorting by Merge sort finished. {wasted_time} seconds were spent')
    #return arr



###################### Insertion Sort ######################
def insertion_sort(arr): 
    for i in range(len(arr)):
        cursor = arr[i]
        pos = i
        while pos > 0 and arr[pos - 1] > cursor:
            # Меняем местами число, продвигая по списку
            arr[pos] = arr[pos - 1]
            pos = pos - 1
        # Остановимся и сделаем последний обмен
        arr[pos] = cursor
    return arr


def I_S(arr):
    print('Sorting by Insertion sort started')
    time_of_start = time.time() 
    insertion_sort(arr)
    wasted_time = time.time() - time_of_start
    print(f'Sorting by Insertion sort finished. {wasted_time} seconds were spent')
    #eturn arr


def main():
    print(f'Len of list is {LEN_OF_LIST}')
    raw_list = make_list()
    proc_for_QS = Process(target=Q_S, args=([raw_list.copy(),]))
    proc_for_MS = Process(target=M_S, args=([raw_list.copy(),]))
    proc_for_IS = Process(target=I_S, args=([raw_list.copy(),]))
    proc_for_QS.start()
    proc_for_MS.start()
    proc_for_IS.start()


if __name__ == "__main__":
    main()