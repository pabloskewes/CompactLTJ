#ifndef CLTJ_COMPACT_TRIE_IV_H
#define CLTJ_COMPACT_TRIE_IV_H

#include <cltj_regular_trie.hpp>
#include <fstream>
#include <iostream>
#include <queue>
#include <sdsl/select_support_mcl.hpp>
#include <sdsl/vectors.hpp>
#include <string>
#include <vector>

namespace cltj {

class compact_trie {
public:
  typedef uint64_t size_type;
  typedef uint32_t value_type;

private:
  sdsl::bit_vector m_bv;
  sdsl::int_vector<> m_seq;
  sdsl::rank_support_v<1> m_rank1;
  sdsl::select_support_mcl<0> m_select0;

  void copy(const compact_trie &o) {
    m_bv = o.m_bv;
    m_seq = o.m_seq;
    m_rank1 = o.m_rank1;
    m_rank1.set_vector(&m_bv);
    m_select0 = o.m_select0;
    m_select0.set_vector(&m_bv);
  }

  inline size_type rank0(const size_type i) const {
    return i - m_rank1(i);
  }

public:
  const sdsl::int_vector<> &seq = m_seq;

  compact_trie() = default;

  compact_trie(const Trie *trie, const uint64_t n_nodes) {
    auto total_bits = 2 * n_nodes + 1; // 2*n_nodes+1
    m_bv = sdsl::bit_vector(total_bits, 1);
    std::vector<uint32_t> s;
    s.reserve(n_nodes);

    const Trie *node;
    std::queue<const Trie *> q;
    q.push(trie);
    uint64_t pos_bv = 1;
    m_bv[1] = 0;
    while (!q.empty()) {
      node = q.front();
      q.pop();
      for (const auto &child : node->children) {
        s.push_back(child.first);
        q.push(child.second);
      }
      pos_bv = pos_bv + node->children.size() + 1;
      // TODO: por ahora +1, despois quitar esto e ollo cos calculos
      m_bv[pos_bv] = 0;
    }
    std::cout << "n_nodes=" << n_nodes << " s.size()=" << s.size()
              << " pos_bv=" << pos_bv << std::endl;
    // TODO: penso que podo reducir e non ter que gardar os 0s das follas

    m_seq = sdsl::int_vector<>(n_nodes);
    for (auto i = 0; i < n_nodes; ++i) {
      m_seq[i] = s[i];
    }
    // Clear s
    s.clear();
    s.shrink_to_fit();

    sdsl::util::bit_compress(m_seq);
    sdsl::util::init_support(m_rank1, &m_bv);
    sdsl::util::init_support(m_select0, &m_bv);
  }

  //! Copy constructor
  compact_trie(const compact_trie &o) {
    copy(o);
  }

  //! Move constructor
  compact_trie(compact_trie &&o) {
    *this = std::move(o);
  }

  //! Copy Operator=
  compact_trie &operator=(const compact_trie &o) {
    if (this != &o) {
      copy(o);
    }
    return *this;
  }

  //! Move Operator=
  compact_trie &operator=(compact_trie &&o) {
    if (this != &o) {
      m_bv = std::move(o.m_bv);
      m_seq = std::move(o.m_seq);
      m_rank1 = std::move(o.m_rank1);
      m_rank1.set_vector(&m_bv);
      m_select0 = std::move(o.m_select0);
      m_select0.set_vector(&m_bv);
    }
    return *this;
  }

  void swap(compact_trie &o) {
    // m_bp.swap(bp_support.m_bp); use set_vector to set the supported
    // bit_vector
    std::swap(m_bv, o.m_bv);
    std::swap(m_seq, o.m_seq);
    sdsl::util::swap_support(m_rank1, o.m_rank1, &m_bv, &o.m_bv);
    sdsl::util::swap_support(m_select0, o.m_select0, &m_bv, &o.m_bv);
  }

  /*
      Receives index in bit vector
      Returns index of next 0
  */
  uint32_t succ0(uint32_t it) {
    return m_select0(rank0(it) + 1);
  }

  /*
      Receives index of current node and the child that is required
      Returns index of the nth child of current node
  */
  uint32_t child(uint32_t it, uint32_t n) {
    return m_select0(m_rank1(it + n)) + 1;
  }

  /*
      Receives index of node whos children we want to count
      Returns how many children said node has
  */
  uint32_t childrenCount(uint32_t it) {
    return succ0(it) - it;
  }

  inline size_type nodemap(size_type i) const {
    return rank0(i) - 2;
  }

  inline size_type nodeselect(size_type i) const {
    return m_select0(i + 2) + 1;
  }

  pair<uint32_t, uint32_t>
  binary_search_seek(uint32_t val, uint32_t i, uint32_t f) const {
    if (m_seq[f] < val)
      return make_pair(0, f + 1);
    uint32_t mid;
    while (i < f) {
      mid = (i + f) / 2;
      if (m_seq[mid] < val) {
        i = mid + 1;
      } else {
        f = mid;
      }
    }
    return make_pair(m_seq[i], i);
  }

  //! Serializes the data structure into the given ostream
  size_type serialize(
      std::ostream &out,
      sdsl::structure_tree_node *v = nullptr,
      std::string name = ""
  ) const {
    sdsl::structure_tree_node *child =
        sdsl::structure_tree::add_child(v, name, sdsl::util::class_name(*this));
    size_type written_bytes = 0;
    written_bytes += m_bv.serialize(out, child, "bv");
    written_bytes += m_seq.serialize(out, child, "seq");
    written_bytes += m_rank1.serialize(out, child, "rank1");
    written_bytes += m_select0.serialize(out, child, "select0");
    sdsl::structure_tree::add_size(child, written_bytes);
    return written_bytes;
  }

  void load(std::istream &in) {
    m_bv.load(in);
    m_seq.load(in);
    m_rank1.load(in, &m_bv);
    m_select0.load(in, &m_bv);
  }
};
} // namespace cltj
#endif