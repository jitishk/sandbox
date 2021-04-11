#include <iostream>
#include <vector>
#include <stack>
//using std;
using namespace std;
void
printvector (vector<int> &vect, int i) {
    cout << i << ": ";
    for (auto val : vect) {
        cout << val << " ";
    }
    cout << endl;
}
int zero_matrix(std::vector<std::vector<int> > a) {
    int n = a.size();
    int m = a[0].size();

    int ans = 0;
    std::vector<int> d(m, -1), d1(m), d2(m);
    std::stack<int> st;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == 1)
                d[j] = i;
        }
        printvector(d, i);
        for (int j = 0; j < m; ++j) {
            while (!st.empty() && d[st.top()] <= d[j])
                st.pop();
            d1[j] = st.empty() ? -1 : st.top();
            st.push(j);
        }
        printvector(d1, i);
        while (!st.empty())
            st.pop();

        for (int j = m - 1; j >= 0; --j) {
            while (!st.empty() && d[st.top()] <= d[j])
                st.pop();
            d2[j] = st.empty() ? m : st.top();
            st.push(j);
        }
        printvector(d2, i);
        while (!st.empty())
            st.pop();
        cout << endl;
        for (int j = 0; j < m; ++j)
            ans = std::max(ans, (i - d[j]) * (d2[j] - d1[j] - 1));
    }
    return ans;
}

int main(int argc, char *argv[]) {
    std::vector<std::vector<int> > matrix
    {
        {1, 1, 0, 0, 0, 1},
        {1, 1, 0, 0, 1, 1}:,
        {1, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 1},
    };
    
    std::cout << "ans : " << zero_matrix(matrix) << std::endl;
}
