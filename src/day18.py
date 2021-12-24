import math
import queue
from dataclasses import dataclass


@dataclass
class Pair:
    prev: any
    a: any = None
    b: any = None

    def __str__(self):
        res = f"[{self.a},{self.b}]"
        return res

    def a_end(self):
        return isinstance(self.a, int)

    def b_end(self):
        return isinstance(self.b, int)

    def set_value(self, value):
        if self.a is None:
            self.a = value
        else:
            self.b = value

    def explode(self):
        # left value added to first regular to the left
        # right to the first right
        # then the exploding pair is replaced with 0
        counter = 0
        last_node = self
        previous_nodes = []
        while last_node.a != None and isinstance(last_node.a, Pair):
            previous_nodes.append(last_node)
            last_node = last_node.a
            counter += 1

        a = None
        b = None
        for p in previous_nodes[::-1]:
            if a is None and isinstance(p.a, int):
                a = p.a
            if b is None and isinstance(p.b, int):
                b = p.b



def string_to_node(s):
    ptrs = [Pair(None, None, None)]
    # NOTE: 1 offset becaus root node is already created
    i = 1
    while i < len(s) - 1:
        if s[i] == "[":
            new = Pair(ptrs[-1], None, None)
            ptrs[-1].set_value(new)
            ptrs.append(new)
        elif s[i] == "]":
            ptrs = ptrs[:-1]
        elif s[i] == ",":
            pass
        elif s[i].isdigit():
            ptrs[-1].set_value(int(s[i]))
        else:
            assert False, s[i]
        i += 1

    assert len(ptrs) == 1
    root = ptrs[0]
    return root



def do_explode(root, node):

    print("EXPLODE", node)
    a_value = None
    # a
    # Walk up until we find a end node or
    # a node on the left then switch down and search right/b
    it = node
    go_down = False
    a_found = False
    while it.prev:
        if it.prev.a_end():
            it.prev.a += node.a
            a_found = True
            break
        if not it.prev.a_end() and id(it.prev.a) != id(it):
            # go down
            it = it.prev
            go_down = True
            break
        it = it.prev

    if go_down:
        it = it.a
        # if it == root:
        #     it = it.a
        while it.b is not None:
            if it.b_end():
                it.b += node.a
                a_found = True
                break
            it = it.b


    # b
    it = node
    go_down = False
    b_found = False
    while it.prev:
        if it.prev.b_end():
            it.prev.b += node.b
            b_found = True
            break
        elif not it.prev.b_end() and id(it.prev.b) != id(it):
            # go down
            it = it.prev
            go_down = True
            break
        it = it.prev

    if go_down:
        # if it == root:
        #     it = it.b
        it = it.b
        while it.a is not None:
            if it.a_end():
                it.a += node.b
                b_found = True
                break
            it = it.a


    if id(node) == id(node.prev.a):
        node.prev.a = 0
    if id(node) == id(node.prev.b):
        node.prev.b = 0



def do_split(node, value):
    new_a = math.floor(value / 2) 
    new_b = math.ceil(value / 2)
    res = Pair(node, new_a, new_b)
    return res

def traverse_tree(root):
    q = queue.Queue()
    q.put((root, 0))

    # explodes
    go_again = False
    while not q.empty():
        node, depth = q.get()
        # print(node, depth)

        if depth >= 4 and node.a_end() and node.b_end():
            do_explode(root, node)
            go_again = True
            print("EXPLODED", root)
            break

        if not node.a_end():
            q.put((node.a, depth + 1))
        if not node.b_end():
            q.put((node.b, depth + 1))

    if go_again:
        traverse_tree(root)

    go_again = False
    q = queue.Queue()
    q.put(root)
    while not q.empty():
        node = q.get()

        if node.a_end() and node.a >= 10: 
            node.a = do_split(node, node.a)
            print("SPLITTED", root)
            go_again = True
        if node.b_end() and node.b >= 10:
            node.b = do_split(node, node.b)
            print("SPLITTED", root)
            go_again = True


        if not node.a_end():
            q.put(node.a)
        if not node.b_end():
            q.put(node.b)

    if go_again:
        traverse_tree(root)



def add(n1, n2):
    new = Pair(None, n1, n2)
    n1.prev = new
    n2.prev = new
    print("AFTER ADDITION", new)
    traverse_tree(new)
    return new
                


# contents = "[[1,2],[[3,4],5]]"
# contents = "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]"
# explode tests
# contents = "[[[[[9,8],1],2],3],4]"
# contents = "[7,[6,[5,[4,[3,2]]]]]"
# contents = "[[6,[5,[4,[3,2]]]],1]"
# contents = "[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]"
# # split tests
# 
# root = string_to_node(contents)
# print(root)
# traverse_tree(root)
# print(root)

# first = "[[[[4,3],4],4],[7,[[8,4],9]]]"
# second = "[1,1]"
# first = string_to_node(first)
# second = string_to_node(second)
# 
# root = add(first, second)

with open("data/input_day18.txt") as fp:
    nodes = [string_to_node(s) for s in fp.read().split("\n")[:-1]]

if nodes:
    res = nodes[0]
    print(res)
    for n in nodes[1:]:
        res = add(res, n)

print(res)
