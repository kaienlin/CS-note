struct Heap {
    constexpr static int max_size = 5e5;

    int heap_size = 0;
    int node[max_size];

    int parent(int v) { return (v - 1) / 2; }
    int lch(int v) { return (v << 1) + 1; }
    int rch(int v) { return (v << 1) + 2; }

    void heapify(int root) {
        int cur = root, tmp = node[root];
        while (cur < heap_size && lch(cur) < heap_size) {
            int l = lch(cur), r = rch(cur), mn_idx = -1;

            if (r < heap_size)
                mn_idx = node[l] < node[r] ? l : r;
            else
                mn_idx = l;

            if (node[mn_idx] < tmp) {
                node[cur] = node[mn_idx];
                cur = mn_idx;
            } else
                break;
        }
        node[cur] = tmp;
    }

    void init(int* arr, int n) {
        heap_size = n;
        copy(arr, arr+n, node);
        // the last "non-leaf node" is the parent of the last node
        // parent(n-1) = ((n-1) - 1) / 2 = n / 2 - 1
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(i);
    }

    void push(int key) {
        node[heap_size++] = key;
        int cur = heap_size - 1;
        while(cur != 0 && node[parent(cur)] > node[cur]) {
            swap(node[cur], node[parent(cur)]);
            cur = parent(cur);
        }
    }

    int pop() {
        int ret = node[0];
        node[0] = node[--heap_size];
        heapify(0);
        return ret;
    }

    int peek() {
        return node[0];
    }
};
