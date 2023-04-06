#include "rbtree.h"

// #include <assert.h>
// // #include <rbtree.h>
// #include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  // root 초기값 설정
  // black , parent nil , child nil => sentinel node
  NIL->color = RBTREE_BLACK; 
  p->nil = NIL;  
  p->root = NIL;
  return p;
}

void delete_rbtree_nodes(rbtree *t, node_t *x) {
  if (x != t->nil) {
    // DFS로 트리를 순회
    delete_rbtree_nodes(t, x->left);
    delete_rbtree_nodes(t, x->right);
    // 각 노드 해제
    free(x);
  }
}

void delete_rbtree(rbtree *t) {
  // 트리의 모든 노드를 삭제
  delete_rbtree_nodes(t, t->root);

  // 더 이상 필요 없는 메모리 해제
  free(t->nil);
  free(t);
}

void rotate_left(rbtree *t, node_t *p) {
  node_t *right = p->right;
  p->right = right->left;
  if (right->left != t->nil) {
    right->left->parent = p; // right의 부모를 p노드로 지정
  }
  right->parent = p->parent;
  if (p->parent == t->nil) {
    t->root = right;
  } else if (p == p->parent->left) {
    p->parent->left = right;
  } else {
    p->parent->right = right;
  }
  right->left = p;
  p->parent = right;
}

void rotate_right(rbtree *t, node_t *p) {
  node_t *left = p->left;
  p->left = left->right;
  if (left->right != t->nil) {
    left->right->parent = p; // right의 부모를 p노드로 지정
  }
  left->parent = p->parent;
  if (p->parent == t->nil) {
    t->root = left;
  } else if (p == p->parent->right) {
    p->parent->right = left;
  } else {
    p->parent->left = left;
  }
  left->right = p;
  p->parent = left;
}

// case 1 : 노드 z의 삼촌 y가 적색인 경우
// case 2 : 노드 z의 삼촌 y가 흑색이며 노드 z가 오른쪽 자식인 경우
// case 3 : 노드 z의 삼촌 y가 흑색이며 노드 z가 왼쪽 자식인 경우
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;
  
  while (z->parent->color == RBTREE_RED) {
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      
      // CASE 1 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 2 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->right) {
          z = z->parent;
          rotate_left(t, z);
        }
        // CASE 3 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotate_right(t, z->parent->parent);
      }
    }
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    else {
      y = z->parent->parent->left;

      // CASE 4 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 5 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->left) {
          z = z->parent;
          rotate_right(t, z);
        }
        // CASE 6 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotate_left(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert 
  node_t *y = t->nil; // y will be parent of z
  node_t *x = t->root;
  // 노드 크기 만큼의 동적 메모리 할당
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  // 입력받은 key 값 할당
  z->key= key;
  
  while (x != t->nil) {
      y = x; // root부터 시작해 z의 부모 노드가 될 노드 찾음
      if (z->key < x->key) {
          x = x->left;
      }
      // 작으면 오른쪽으로 한칸 내려감
      else {
          x = x->right;
      }
  };
  // nil까지 내려가서 z의 부모값으로 y 할당
  z->parent = y;

  if (y == t->nil){  // x가 값이 없어서 while문 패스시 , 
    t->root = z;  // root 주소에 할당
  }  
  else if (z->key < y->key){   //입력값 z가 부모값 y보다 크면
    y->left = z;   // 왼쪽 
  }
  else {
    y->right = z;  //오른쪽
  };

  // 삽입된 z  child nil , color red 
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  // rbtree 특성 위반 체크 
  rbtree_insert_fixup(t, z);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;
  while (x != t->nil) {
     if (key == x->key) {
        return x;
    } else if (key < x->key) {
        x = x->left;
    } else if (key > x->key) {
        x = x->right;
    } 
  }
  return NULL;
}


node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil){
    return NULL;
  }
  
  node_t *x = t->root;
  while (x->left != t->nil) {
      x = x->left;
  }  
  return x; 
  // return t->root; 
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t->root == t->nil){
    return NULL;
  }

  node_t *x = t->root;
  while (x->right != t->nil) {
      x = x->right;
  }  
  return x; 

  // return t->root;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }
  else if (u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }

  v->parent = u->parent;
}

// 삭제시 규칙 위반 하는 경우
// case 1: x의 형제 w가 적색
// case 2: x의 형제 w는 흑색이고 w의 두 자식이 모두 흑색
// case 3: x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자식은 흑색
// case 4: x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색

void rbtree_erase_fixup(rbtree *t, node_t *x){
  node_t *w;
  while (x!= t->root && x->color == RBTREE_BLACK){
    // case 1-4
    if (x == x->parent->left){
      w = x->parent->right;
      // case 1
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;                    // case 1
        x->parent->color = RBTREE_RED;              // case 1
        rotate_left(t,x->parent);                   // case 1
        w = x->parent->right;                       // case 1
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;                      // case 2
        x = x->parent;                              // case 2
      }

      else {
        if (w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;            // case 3
          w->color = RBTREE_RED;                    // case 3
          rotate_right(t,w);                        // case 3
          w = x->parent->right;                     // case 3
        }
        w->color = x->parent->color;                // case 4              
        x->parent->color = RBTREE_BLACK;            // case 4   
        w->right->color = RBTREE_BLACK;             // case 4   
        rotate_left(t,x->parent);                   // case 4   
        x = t->root;                                // case 4    
      }
    }
    else{
      w = x->parent->left;

      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate_right(t,x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_left(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate_right(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

node_t *tree_minimum(rbtree *t, node_t *x) {
  while (x->left != t->nil)
  {
    x = x->left;
  }

  return x;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y;
  node_t *x;
  color_t y_original_color;

  y = p;
  y_original_color = y->color;

  if(p->left == t->nil){
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil){
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else{
    // p 노드 자식 2개이상
    y = tree_minimum(t, p->right); // 오른쪽 서브트리에 가장 작은값과 교체
    y_original_color = y->color;
    x = y->right;

    if (y->parent == p){
      x->parent = y;
    }
    else{
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }

  free(p);
  return 0;
}

void inorder_traversal(const node_t *x, key_t *arr, size_t *idx, const rbtree *t) {
  if (x != t->nil) {
    inorder_traversal(x->left, arr, idx, t);
    arr[*idx] = x->key;
    (*idx)++;
    inorder_traversal(x->right, arr, idx, t);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t idx = 0;
  inorder_traversal(t->root, arr, &idx, t);
  return 0;
}
