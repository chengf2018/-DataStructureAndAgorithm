#include <cassert>
#include <cinttypes>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

#define K 3 // K degree b+tree

typedef struct _bnode {
  struct _bnode *father;
  struct _bnode *next;
  struct _bnode *last;
  int key[K];
  int key_count;
  struct _bnode *p[K + 1];
  int btype; // 1.internal node 2.leaf node
} bnode;

typedef struct _btree {
  struct _bnode *root;
} btree;

bnode *new_node(int type) {
  bnode *bn = new bnode();
  bn->father = nullptr;
  bn->next = nullptr;
  bn->last = nullptr;
  memset(bn->key, 0, sizeof(bn->key) * sizeof(int));
  bn->key_count = 0;
  for (int i = 0; i < K + 1; i++) {
    bn->p[i] = nullptr;
  }
  bn->btype = type;
  return bn;
}

void free_node(bnode *node) {
  if (!node)
    return;
  // don't need to free (K+1)th node
  for (int i = 0; i < K; i++) {
    if (!node->p[i])
      break;
    free_node(node->p[i]);
  }
  delete node;
}

void init_btree(btree *bt) { bt->root = new_node(2); }

void free_btree(btree *bt) {
  if (!bt)
    return;
  free_node(bt->root);
}

// return key pos or insert pos
static int search_node_key_insert_pos(bnode *bn, int key) {
  int pos;
  for (pos = 0; pos < bn->key_count; pos++) {
    if (key > bn->key[pos])
      break;
  }
  return pos;
}

static int search_node_key(bnode *bn, int key) {
  for (int i = 0; i < bn->key_count; i++) {
    if (key == bn->key[i])
      return i;
  }
  return -1;
}

static void insert_key_to_internal_node(bnode *bn, int pos, bnode *nn,
                                        int key) {
  assert(pos >= 0 && pos <= bn->key_count && pos < K + 1);
  assert(bn->key_count < K);
  for (int i = bn->key_count; i > pos; i--) {
    bn->key[i] = bn->key[i - 1];
    bn->p[i + 1] = bn->p[i];
  }
  bn->key[pos] = key;
  if (bn->key_count == pos) {
    bn->p[pos + 1] = nn;
  } else {
    bn->p[pos] = nn;
  }
  bn->key_count++;
}

static void insert_key_to_leaf_node(bnode *bn, int key, int pos) {
  assert(pos >= 0 && pos < K);
  if (pos < bn->key_count) {
    for (int i = bn->key_count; i > pos; i--) {
      bn->key[i] = bn->key[i - 1];
    }
  }
  bn->key[pos] = key;
  bn->key_count++;
}

static void split_internal_node(btree *bt, bnode *bn) {
  assert(bn->key_count == K);
  // split node = ceil(K / 2)
  int split_node_pos = (K + 1) / 2 - 1;
  // left nodes stay where it is
  // right nodes alloc to where new node
  // split node insert to where father node
  // if not have father node, create new father
  bnode *right_new = new_node(1);
  right_new->last = bn;
  right_new->next = bn->next;
  int right_count = 0;
  for (int i = split_node_pos + 1; i < K; i++) {
    right_new->key[right_count] = bn->key[i];
    right_new->p[right_count] = bn->p[i];
    bn->p[i + 1] = nullptr;
    bn->key_count--;
    bn->key[i] = 0;
    right_count++;
  }
  // don't forget the last p
  right_new->p[right_count + 1] = bn->p[K];
  bn->p[K] = nullptr;
  right_new->key_count = right_count;

  bn->next = right_new;

  bnode *father = bn->father;
  if (!father) {
    father = new_node(1);
    bt->root = father;
  }

  int split_key = bn->key[split_node_pos];
  // remove split key
  bn->key[split_node_pos] = 0;
  bn->key_count--;

  int insert_pos = search_node_key_insert_pos(father, split_key);
  insert_key_to_internal_node(father, insert_pos, right_new, split_key);
  // if father node is full after insert, recursion split it
  if (father->key_count == K) {
    split_internal_node(bt, father);
  }
}

static void insert_leaf_node_to_internal(btree *bt, bnode *bn, bnode *nn) {
  assert(bn->btype == 1);
  assert(nn->key_count > 0);
  int pos = search_node_key_insert_pos(bn, nn->key[0]);
  insert_key_to_internal_node(bn, pos, nn, nn->key[0]);

  // is full?
  if (bn->key_count == K) {
    // split internal node
    split_internal_node(bt, bn);
  }
}

static void split_leaf_node(btree *bt, bnode *bn) {
  assert(bn->btype == 2);
  assert(bn->key_count == K);

  // split node
  // floor(K/2) node stay where it is
  // ceil(K/2) node alloc to where new node
  bnode *right_new = new_node(2);
  right_new->last = bn;
  right_new->next = bn->next;
  int right_count = 0;
  for (int i = K / 2; i < bn->key_count; i++) {
    right_new->key[right_count] = bn->key[i];
    right_new->p[right_count] = bn->p[i];
    right_count++;
    bn->key[i] = 0;
    bn->p[i] = nullptr;
  }
  bn->key_count = K / 2;
  right_new->key_count = right_count;

  // right node inherit left node p[K]
  // left node p[K] link to right node
  right_new->p[K] = bn->p[K];
  bn->p[K] = right_new;

  bn->next = right_new;

  if (!bn->father) {
    // when no father, create new father node
    // and insert first key of right node
    // link left node and right node to where p[0] and p[1] in father
    // reset bt root point
    bnode *father = new_node(1);
    father->key[0] = right_new->key[0];
    father->key_count = 1;
    father->p[0] = bn;
    father->p[1] = right_new;
    bn->father = father;
    right_new->father = father;
    bt->root = father;
  } else {
    insert_leaf_node_to_internal(bt, bn->father, right_new);
  }
}

static void insert_to_leaf(btree *bt, bnode *bn, int key) {
  assert(bn->btype == 2);

  int pos = search_node_key_insert_pos(bn, key);
  insert_key_to_leaf_node(bn, key, pos);

  // Is full?
  if (bn->key_count == K) {
    split_leaf_node(bt, bn);
  }
}

static bnode *search_insert_leaf(btree *bt, int key) {
  bnode *bn = bt->root;
  assert(bn);
  while (bn) {
    if (bn->btype == 2)
      return bn;
    assert(bn->key_count > 0);
    int pos = search_node_key_insert_pos(bn, key);
    bn = bn->p[pos];
  }
  assert(false);
  return bn;
}

void btree_insert(btree *bt, int key) {
  bnode *leaf = search_insert_leaf(bt, key);
  insert_to_leaf(bt, leaf, key);
}

static void remove_leaf_key(bnode *leaf, int pos) {
  assert(leaf->key_count > 0);
  assert(pos >= 0 && pos < leaf->key_count);

  for (int i = pos; i < leaf->key_count - 1; i++) {
    leaf->key[i] = leaf->key[i + 1];
  }
  leaf->key[leaf->key_count - 1] = 0;
  leaf->key_count--;
}

static void shift_leaf_father_key(bnode *leaf, int key, int set_key) {
  bnode *father = leaf->father;
  while (father) {
    int father_key_pos = search_node_key(father, key);
    if (father_key_pos != -1) {
      father->key[father_key_pos] = set_key;
      break;
    }
    father = father->father;
  }
}

static void remove_internal_key(bnode *bn, int pos) {
  assert(pos < bn->key_count);
  if (pos == 0) {
    for (int i = pos; i < bn->key_count - 1; i++) {
      bn->key[i] = bn->key[i + 1];
      bn->p[i] = bn->p[i + 1];
    }
  } else {
    for (int i = pos; i < bn->key_count - 1; i++) {
      bn->key[i] = bn->key[i + 1];
      bn->p[i + 1] = bn->p[i + 2];
    }
  }
  bn->key_count--;
}

static int search_internal_node_p(bnode *bn, bnode *p) {
  assert(bn && p);
  for (int i = 0; i < bn->key_count; i++) {
    if (bn->p[i] == p) {
      return i;
    }
  }
  return -1;
}

static void merge_leaf_to_root(btree *bt) {
  bnode *bn = bt->root;
  assert(bn->key_count > 0);
  bnode *new_leaf = new_node(2);
  for (int i = 0; i <= bn->key_count; i++) {
    bnode *leaf = bn->p[i];
    for (int j = 0; j < leaf->key_count; j++) {
      insert_key_to_leaf_node(new_leaf, leaf->key[j], new_leaf->key_count);
    }
    delete leaf;
  }
  delete bt->root;
  bt->root = new_leaf;
}

static void shift_up_internal_node(btree *bt, bnode *bn) {
  if (bt->root == bn) {
    if (bn->key_count <= 1) {
      merge_leaf_to_root(bt);
    }
    return;
  }

  bnode *father = bn->father;
  assert(father);
  // find father index
  int father_p_pos = search_internal_node_p(father, bn);
  assert(father_p_pos != -1);

  // prioritize borrowing from brother nodes
  if (father_p_pos > 0 && father->p[father_p_pos - 1]->key_count > K / 2) {
    // has left and have enough nodes
    // fahter key down to current node
    // left last node up to father
    bnode *brother = father->p[father_p_pos - 1];
    int brother_last_node_key = brother->key[brother->key_count - 1];
    bnode *brother_last_node_p = brother->p[brother->key_count];
    int father_key = father->key[father_p_pos - 1];
    insert_key_to_internal_node(bn, 0, brother_last_node_p, father_key);
    father->key[father_p_pos - 1] = brother_last_node_key;
    brother->p[brother->key_count] = nullptr;
    brother->key[brother->key_count - 1] = 0;
    brother->key_count--;
  } else if (father_p_pos < father->key_count &&
             father->p[father_p_pos + 1]->key_count > K / 2) {
    // has right and have enough node
    bnode *brother = father->p[father_p_pos + 1];
    int brother_frist_node_key = brother->key[0];
    bnode *brother_first_node_p = brother->p[0];
    int father_key = father->key[father_p_pos];
    insert_key_to_internal_node(bn, bn->key_count, brother_first_node_p,
                                father_key);
    father->key[father_p_pos] = brother_frist_node_key;
    for (int i = 0; i < brother->key_count - 1; i++) {
      brother->key[i] = brother->key[i + 1];
      brother->p[i] = brother->p[i + 1];
    }
    brother->p[brother->key_count - 1] = brother->p[brother->key_count];
    brother->p[brother->key_count] = nullptr;
    brother->key_count--;
  } else if (father_p_pos > 0) {
    // has left but not enough nodes
    bnode *brother = father->p[father_p_pos - 1];
    int father_key = father->key[father_p_pos - 1];
    insert_key_to_internal_node(brother, brother->key_count, bn->p[0],
                                father_key);
    father->p[father_p_pos] = father->p[father_p_pos - 1];
    remove_internal_key(father, father_p_pos - 1);
    for (int i = 0; i < bn->key_count; i++) {
      insert_key_to_internal_node(brother, brother->key_count, bn->p[i + 1],
                                  bn->key[i]);
    }
    delete bn;
    if (father->key_count < K / 2) {
      shift_up_internal_node(bt, father);
    }
  } else if (father_p_pos < father->key_count) {
    // has right but not enough nodes
    bnode *brother = father->p[father_p_pos + 1];
    int father_key = father->key[father_p_pos];
    insert_key_to_internal_node(bn, bn->key_count, brother->p[0], father_key);
    remove_internal_key(father, father_p_pos);
    for (int i = 0; i < brother->key_count; i++) {
      insert_key_to_internal_node(bn, bn->key_count, brother->p[i + 1],
                                  brother->key[i]);
    }
    delete brother;
    if (father->key_count < K / 2) {
      shift_up_internal_node(bt, father);
    }
  } else {
    assert(false); // if node have father, then it must have brother
  }
}

static void merge_leaf_node_to_left(btree *bt, bnode *leaf, bnode *brother,
                                    int remove_key) {
  bnode *father = leaf->father;
  assert(father);
  // merge leaf to brother
  if (leaf->key_count > 0) {
    for (int i = 0; i < leaf->key_count; i++) {
      insert_key_to_leaf_node(brother, leaf->key[i], brother->key_count);
    }
    int remove_pos = search_node_key(father, leaf->key[0]);
    remove_internal_key(father, remove_pos);
  } else {
    int remove_pos = search_node_key(father, remove_key);
    remove_internal_key(father, remove_pos);
  }
  if (father->key_count < K / 2) {
    shift_up_internal_node(bt, father);
  }
  brother->next = leaf->next;
  delete leaf;
}

static void merge_leaf_node_to_right(btree *bt, bnode *leaf, bnode *brother,
                                     int remove_key) {
  bnode *father = leaf->father;
  assert(father);
  // merge leaf to right brother
  // what kind of situation need to merge to right? when remove key in first
  // leaf
  int remove_pos = search_node_key(brother, brother->key[0]);
  // assert(remove_pos != -1);
  assert(remove_pos == 0);
  remove_internal_key(father, remove_pos);

  for (int i = 0; i < leaf->key_count; i++) {
    insert_key_to_leaf_node(brother, leaf->key[i], brother->key_count);
  }

  if (father->key_count < K / 2) {
    shift_up_internal_node(bt, father);
  }

  brother->last = leaf->last;
  delete leaf;
}
int btree_remove(btree *bt, int key) {
  bnode *leaf = search_insert_leaf(bt, key);
  int pos = search_node_key(leaf, key);

  // if not found?
  if (pos == -1)
    return 0;

  remove_leaf_key(leaf, pos);
  if (leaf == bt->root) {
    return 1;
  }

  // if removed key is leaf first key, change its father key
  if (pos == 0 && leaf->key_count > 0) {
    shift_leaf_father_key(leaf, key, leaf->key[0]);
  }

  if (leaf->key_count < K / 2 && leaf->father) {
    // brother have enough key?
    bnode *brother = nullptr;
    bnode *father = leaf->father;
    if (leaf->next and leaf->next->key_count > K / 2) {
      brother = leaf->next;
      int f_pos = search_node_key(father, brother->key[0]);
      assert(f_pos != -1);
      father->key[f_pos] = brother->key[1];
      int key_0 = brother->key[0];
      remove_leaf_key(brother, 0);
      insert_key_to_leaf_node(leaf, key_0, leaf->key_count);

      if (leaf->key_count == 1) {
        shift_leaf_father_key(leaf, key, key_0);
      }
    } else if (leaf->last and leaf->last->key_count > K / 2) {
      brother = leaf->last;
      insert_key_to_leaf_node(leaf, brother->key[brother->key_count], 0);
      remove_leaf_key(brother, brother->key_count - 1);
      shift_leaf_father_key(leaf, key, leaf->key[0]);
    } else {
      // brother not enough key
      // merge it
      if (leaf->last)
        merge_leaf_node_to_left(bt, leaf, leaf->last, key);
      else if (leaf->next)
        merge_leaf_node_to_right(bt, leaf, leaf->next, key);
      else
        assert(false);
    }
  }
  return 1;
}

std::vector<int> btree_find(btree *bt, int left, int right) {
  bnode *bn = bt->root;
  vector<int> result;
  while (bn) {
    if (bn->btype == 2) {
      int not_match = 0;
      for (int i = 0; i < bn->key_count; i++) {
        int key = bn->key[i];
        if (key >= left && key <= right) {
          result.push_back(key);
          std::cout << "find:" << key << std::endl;
        } else {
          not_match = 1;
          break;
        }
      }
      if (not_match) {
        break;
      }
      bn = bn->next;
    } else {
      int i = 0;
      for (i = 0; i < bn->key_count; i++) {
        int key = bn->key[i];
        if (key > left) {
          bn = bn->p[i];
          break;
        }
      }
      if (i == bn->key_count) {
        bn = bn->p[bn->key_count];
      }
    }
  }
  return result;
}

int main(int argc, char **argv) {
  btree bt;
  init_btree(&bt);
  btree_insert(&bt, 1);
  btree_insert(&bt, 2);
  btree_insert(&bt, 3);
  btree_insert(&bt, -123);
  std::vector<int> result = btree_find(&bt, 1, 3);
  for (auto &i : result) {
    std::cout << i << std::endl;
  }
  free_btree(&bt);
  std::cout << "test";
  return 0;
}
