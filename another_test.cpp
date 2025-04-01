#include <iostream>
#include <vector>
#include <limits>
using namespace std;

class Item {
    public:
        int value;  // Giá trị số nguyên
        Item(int v) : value(v) {}
    };
    
// Cấu trúc lưu thông tin truy vết: chỉ số phần tử và tổng trước đó.
struct ParentInfo {
    int idx;
    int prevSum;
};

// Tự tạo lớp MyHashMap thay thế cho unordered_map<int, ParentInfo>
template<typename Key, typename Value>
class MyHashMap {
private:
    // Sử dụng một số nguyên tố làm kích thước bảng băm.
    static const int SIZE = 10007;
    vector<vector<pair<Key, Value>>> table;
    
    // Hàm băm đơn giản (chỉ áp dụng cho kiểu int)
    int hashFunc(const Key &key) const {
        return key % SIZE;
    }
    
public:
    MyHashMap() : table(SIZE) {}
    
    // Tìm kiếm key, trả về pointer tới value nếu tìm thấy, ngược lại trả về nullptr.
    Value* find(const Key &key) {
        int h = hashFunc(key);
        for(auto &p : table[h]) {
            if(p.first == key)
                return &p.second;
        }
        return nullptr;
    }
    
    // Kiểm tra key có tồn tại không.
    bool contains(const Key &key) {
        return find(key) != nullptr;
    }
    
    // Chèn key và value nếu key chưa tồn tại.
    void insert(const Key &key, const Value &value) {
        if(!contains(key)) {
            int h = hashFunc(key);
            table[h].push_back({key, value});
        }
    }
    
    // Phương thức operator[] hỗ trợ truy cập và chèn mặc định nếu key chưa tồn tại.
    Value& operator[](const Key &key) {
        int h = hashFunc(key);
        for(auto &p : table[h]) {
            if(p.first == key)
                return p.second;
        }
        table[h].push_back({key, Value()});
        return table[h].back().second;
    }
    
    // Lấy tất cả các mục (entries) trong hash map dưới dạng vector các cặp key - value.
    vector<pair<Key, Value>> entries() const {
        vector<pair<Key, Value>> res;
        for(const auto &bucket : table) {
            for(const auto &entry : bucket) {
                res.push_back(entry);
            }
        }
        return res;
    }
};

//
// Hàm tìm tập con có tổng nhỏ nhất mà lớn hơn n sử dụng "sparse DP" dựa trên MyHashMap.
//
struct DPResult {
    int targetSum; // Tổng nhỏ nhất > n
    vector<int> subset; // Tập con tìm được
};

DPResult findSubsetOptimized(const vector<Item>& arr, int n) {
    int m = arr.size();
    MyHashMap<int, ParentInfo> dp;
    dp.insert(0, {-1, -1}); // cơ sở: tổng 0 có thể tạo ra mà không dùng phần tử nào
    
    // Cập nhật các tổng khả dĩ bằng cách duyệt từng phần tử của mảng
    for (int i = 0; i < m; i++) {
        // Lấy snapshot các tổng hiện có để không bị ảnh hưởng khi cập nhật đồng thời
        vector<int> currentSums;
        auto entries = dp.entries();
        for (auto &entry : entries) {
            currentSums.push_back(entry.first);
        }
        for (int s : currentSums) {
            int newSum = s + arr[i].value;  // Lấy giá trị từ class Item
            if (!dp.contains(newSum)) {
                dp.insert(newSum, {i, s});
            }
        }
    }
    
    // Tìm tổng nhỏ nhất > n
    int best = numeric_limits<int>::max();
    auto entries = dp.entries();
    for (auto &entry : entries) {
        int sumVal = entry.first;
        if (sumVal > n && sumVal < best) {
            best = sumVal;
        }
    }
    
    DPResult result;
    if (best == numeric_limits<int>::max()) {
        return result; // Không tìm được tập con phù hợp
    }
    
    result.targetSum = best;
    // Truy vết lại các phần tử đã chọn
    int cur = best;
    while (cur > 0) {
        ParentInfo info = *dp.find(cur);
        result.subset.push_back(arr[info.idx].value);  // Lấy giá trị từ class Item
        cur = info.prevSum;
    }
    
    return result;
}

// Hàm in kết quả
void printSubset(const DPResult& result) {
    if (result.targetSum == -1) {
        cout << "Khong the tim duoc subset co tong > n" << endl;
        return;
    }
    
    cout << "Subset duoc chon (cac phan tu): ";
    for (int num : result.subset) {
        cout << num << " ";
    }
    cout << "\nTong cua subset: " << result.targetSum << endl;
}

//
// Hàm main để chạy chương trình
int main() {
    vector<Item> items = {Item(3), Item(1), Item(4), Item(8), Item(2)};
    int n = 6;

    DPResult result = findSubsetOptimized(items, n);

    cout << "Tổng nhỏ nhất lớn hơn " << n << " là: " << result.targetSum << endl;
    cout << "Các phần tử được chọn: ";
    for (int v : result.subset) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
