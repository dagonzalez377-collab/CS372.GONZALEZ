# AI Tool Review of the Tree Class (Mod 2 Assignment, Item 3)

**Tool used:** Claude (Anthropic), in the course of getting help refactoring
`List.hpp` and adding the parent pointer to `Tree.hpp`.

## Prompt

Working from the provided `Tree.hpp` (the immutable/persistent binary
search tree using `shared_ptr<const Node>`), I asked for help adding a
parent pointer to `Node` per the assignment instructions, and asked the
tool to flag any strengths, weaknesses, or potential problems it noticed
in the class while doing that work.

## Output / Findings

**Strengths**

- The persistent-tree design (each `insert` builds a new tree while sharing
  unchanged subtrees via `shared_ptr<const Node>`) is a clean way to get
  immutability without copying the whole tree on every operation.
- The `Compare` template parameter on `insert`/`member`/`find` is a nice
  touch -- it lets the same class support ascending or descending ordering,
  or an arbitrary partial order, without duplicating code.
- Recursive `preorder`/`inorder`/`postorder` are about as simple as this
  kind of traversal gets, and they compose well with `operator<<`.

**Weaknesses / bugs found**

1. **Missing `#include <iostream>`.** `operator<<` uses `std::ostream` but
   the header never includes `<iostream>` directly -- it only compiled in
   the original project because something else pulled it in transitively.
   Fixed by adding the include directly, since a header shouldn't depend on
   whoever includes it first to also drag in what it needs.

2. **The default comparator inverts the tree.** The class comment says
   *"the left subtree contains values less than the root, and the right
   subtree contains values greater than the root,"* but `insert`'s default
   `Compare` is `std::greater<T>`. Tracing through `insert`, that actually
   makes **larger** values go left and **smaller** values go right --
   the opposite of the documented behavior and the opposite of a
   conventional BST. Not something I changed (changing default behavior
   could break other code depending on it), but worth knowing: callers who
   want a normal ascending BST need to explicitly pass `std::less<T>()`.

**Potential problems with adding a parent pointer specifically**

This is the interesting one. Because the tree is *persistent* -- `insert`
never mutates in place, it builds a new tree and reuses whatever subtrees
didn't change -- a single `Node` can legitimately be shared by more than
one `Tree` at the same time. That means "the parent" of a node isn't a
fixed fact about the node; it depends on *which* `Tree` you're looking at
it through. A naive `Node* parent` field would only ever reflect the last
tree that happened to attach that node as a child, silently going stale
for every other tree still sharing it.

Two ways to handle this were considered:
- Add the pointer anyway, document that it only reflects the most
  recently-built tree, and accept that tension.
- Store parent info outside the node entirely (e.g., build it on demand
  while walking down from a specific root), which is more correct but a
  bigger design change than the assignment asks for.

I went with the first option, since it's what the assignment describes,
but implemented it as `mutable std::weak_ptr<const Node>` rather than a
raw or owning pointer, for two reasons:
- `weak_ptr` avoids creating a parent-owns-child-owns-parent reference
  cycle with the existing `shared_ptr<const Node>` children, which would
  otherwise leak memory (shared_ptr cycles are never collected).
- `mutable` is needed because the parent link has to be set through a
  `shared_ptr<const Node>` (children are always accessed as `const`), even
  though the node's actual value and children stay logically immutable.

## Code changes made in response

- Added `mutable std::weak_ptr<const Node> _parent;` to `Node`.
- In the `Tree(Tree lft, T val, Tree rgt)` constructor, set `lft._root`'s
  and `rgt._root`'s `_parent` to the newly created node. Since `insert`
  is built entirely out of calls to this same constructor, parent pointers
  for the *resulting* tree end up correct automatically, with no changes
  needed to `insert` itself.
- Added public `parent()` (returns an empty `Tree` for the root, matching
  the "parent of root is nullptr" requirement) and `hasParent()`.
- Added `#include <iostream>` to fix the missing dependency noted above.
