import math
import queue
from dataclasses import dataclass


@dataclass
class Pair:
    value: any = None
    left: any = None
    right: any = None
    prev: any = None


    def is_valid(self):
        return not all((x is None for x in (self.value, self.left, self.right)))

    def is_value(self):
        if self.value is not None:
            assert self.left is None
            assert self.right is None
        return self.value is not None

    def is_node(self):
        return self.value is None and self.left is not None and self.right is not None

    def has_just_values(self):
        res = True
        res = res and self.left is not None and self.left.is_value()
        res = res and self.right is not None and self.right.is_value()
        return res

    def __repr__(self):
        if self.value is not None:
            assert self.left is None
            assert self.right is None
            return f"V({str(self.value)})"
        else:
            res = f"(l={self.left}" + f",r={self.right})"
            return res

    def tostring(self):
        if self.has_just_values():
            res = f"[{self.left.value},{self.right.value}]"
        elif self.is_value():
            res = str(self.value)
        else:
            res = f"[{self.left.tostring()},{self.right.tostring()}]"
        return res


def string_to_node(s):
    pairs = []
    # NOTE: 1 offset becaus root node is already created
    use_left = True
    i = 0
    res = None
    while i < len(s):
        if s[i] == "[":
            new = Pair(None, None, None, None)
            if pairs:
                if use_left:
                    pairs[-1].left = new
                    new.prev = pairs[-1]
                else:
                    pairs[-1].right = new
                    new.prev = pairs[-1]
            else:
                root = new
            use_left = True
            pairs.append(new)
        elif s[i] == "]":
            pairs = pairs[:-1]
        elif s[i] == ",":
            use_left = False
        elif s[i].isdigit():
            value = int(s[i])
            if use_left:
                pairs[-1].left = Pair(value, None, None, pairs[-1])
            else:
                pairs[-1].right = Pair(value, None, None, pairs[-1])
        else:
            assert False, s[i]
        i += 1

    assert root
    return root



def go_up(node, attr):
    it = node
    last_it = None
    while it:
        if it is not node:
            side_value = getattr(it, attr)
            if side_value.is_value():
                break
            if side_value.is_node() and last_it is not None and side_value is not last_it:
                break
        if it.prev is None:
            break
        last_it = it
        it = it.prev
    return it, last_it

def go_down(node, search_for, next_side):
    it = node
    res = None
    while it:
        if getattr(it, search_for).is_value():
            res = it
            break
        it = getattr(it, search_for)
    return res




def do_explode(root, node):

    # NOTES: cases
    # 1. go up until root and nothing is found on search side
    #    - do nothing
    # 2. go up unitl root the search side is a value
    #    - update iter node search side to explode value
    # 3. go up unitl root and search side is a node
    #    - break at node
    #    - go down and search other side for value
    #    - update iter node other side to explode value

    assert node.prev
    # -- left
    # go up
    it, came_from = go_up(node, "left")
    # print("go_up result", it)
    if it.left.is_value():
        it.left.value += node.left.value
    else:
        if it.left is not came_from:
            it = it.left
            # print("Go one down", it)
            it = go_down(it, search_for="right", next_side="left")
            it.right.value += node.left.value

    # -- right
    # go up
    it, came_from = go_up(node, "right")
    if it.right.is_value():
        it.right.value += node.right.value
    else:
        if it.right is not came_from:
            it = it.right
            it = go_down(it, search_for="left", next_side="right")
            it.left.value += node.right.value


    node.left = None
    node.right = None
    node.value = 0


def do_split(node):
    node.left = Pair(math.floor(node.value / 2), None, None, node)
    node.right = Pair(math.ceil(node.value / 2), None, None, node)
    node.value = None
    assert node.prev

def reduce(root):
    q = [(root, 0)]

    # explodes
    go_again = False
    while len(q):
        node, depth = q[-1]
        q = q[:-1]
        # print("GOT NODE", node, depth)
        if node is not root:
            assert node.prev is not None, node

        if depth >= 4 and node.has_just_values():
            # print("GOING TO EXPLODE", node.tostring())
            do_explode(root, node)
            go_again = True
            # print("EXPLODED", root.tostring())
            assert node.prev
            break

        if not node.is_value():
            q.append((node.right, depth + 1))
            q.append((node.left, depth + 1))

    if go_again:
        reduce(root)

    q = [root]
    while len(q) > 0:
        node = q.pop()
        if node is not root:
            assert node.prev is not None

        if node.is_value() and node.value >= 10: 
            # print("DO SPLIT", node)
            do_split(node)
            # print("SPLITTED", root.tostring())
            reduce(root)


        if not node.is_value():
            q.append(node.right)
            q.append(node.left)


def magnitude(node):
    if node.is_value():
        return node.value
    else:
        res = magnitude(node.left) * 3
        res += magnitude(node.right) * 2
        return res


def add(n1, n2):
    new = Pair(None, n1, n2)
    n1.prev = new
    n2.prev = new
    # print("ADDED", new.tostring())
    reduce(new)
    return new
                


tests = [
#     # "[[3,4],1]",
#     # "[[1,2],[[3,4],5]]",
#     # "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]",
#     # [[[[[9,8],1],2],3],4]",
#     # [7,[6,[5,[4,[3,2]]]]]",
#     # "[[6,[5,[4,[3,2]]]],1]",
#     # "[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]",
#     # "[[3,[2,[1,[7,3]]]],[6,3]]",
#  "[[10,[[0,[9,3]],[[6,3],[8,8]]]],1]"
  #"[[[[4,0],[5,4]],[[7,0],[[7,8],5]]],[[5,5],[[[5,6],9],[[5,6],0]]]]"
  # "[[[[4,0],[5,4]],[[7,0],[[7,8],5]]],1]",
    #"[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]",
    #"[[[[4,0],[5,4]],[[7,0],[[6,7],0]]],[1,[[0,1],[9,1]]]]",
    # "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]",
]

# for t in tests:
#     root = string_to_node(t)
#     print("-", root.tostring())
#     reduce(root)
#     print(t, "-",  root.tostring())
# 
# import sys
# sys.exit()


# first = "[[[[4,3],4],4],[7,[[8,4],9]]]"
# second = "[1,1]"
# first = string_to_node(first)
# second = string_to_node(second)
# 
# root = add(first, second)


assert magnitude(Pair(None, Pair(9), Pair(1))) == 29

with open("data/input_day18.txt") as fp:
    nodes = [string_to_node(s) for s in fp.read().split("\n")[:-1]]

print(nodes[0].tostring())
if nodes:
    res = nodes[0]
    for n in nodes[1:]:
        res = add(res, n)

print(res.tostring())
print(magnitude(res))
