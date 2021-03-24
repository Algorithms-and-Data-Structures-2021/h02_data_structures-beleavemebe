#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

    void LinkedList::Add(Element e) {
        auto node = new Node(e, nullptr);
        if (head_ == nullptr) {
            head_ = node;
            tail_ = head_;
            size_ = 1;
        } else {
            tail_ -> next = node;
            tail_ = node;
            size_++;
        }
    }

    void LinkedList::Insert(int index, Element e) {
        internal::check_out_of_range(index, 0, size_ + 1);
        if (head_ == nullptr) {
            auto node = new Node(e, nullptr);
            head_ = node;
            tail_ = node;
            size_ = 0;
        } else if (index == 0) {
            Node *node = new Node(e, head_);
            head_ = node;
        } else if (index == size_) {
            Add(e);
            return;
        } else {
            Node *n = find_node(index - 1);
            auto new_Node = new Node(e, n -> next);
            n -> next = new_Node;
        }
        size_++;
    }

    void LinkedList::Set(int index, Element e) {
        internal::check_out_of_range(index, 0, size_);
        auto node = find_node(index);
        node -> data = e;
    }

    Element LinkedList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        Node *nodeToDelete;
        Element e;
        if (index == 0) {
            nodeToDelete = head_;
            e = head_ -> data;
            head_ = head_ -> next;
        } else {
            Node *prev_Node = find_node(index - 1);
            nodeToDelete = prev_Node -> next;
            e = nodeToDelete -> data;
            prev_Node -> next = nodeToDelete -> next;
        }
        size_--;
        delete nodeToDelete;
        return e;
    }

    void LinkedList::Clear() {
        Node *current = head_;
        for (int i = 0; i < size_; ++i) {
            auto next = current -> next;
            delete current;
            current = next;
        }
        delete current;
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    Element LinkedList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);
        auto node = find_node(index);
        return node -> data;
    }

    int LinkedList::IndexOf(Element e) const {
        auto node = head_;
        for (int i = 0; i < size_; ++i) {
            if (node -> data == e) return i;
             node = node -> next;
        }
        return kNotFoundElementIndex;
    }

    Node *LinkedList::find_node(int index) const {
        assert(index >= 0 && index < size_);
        if (index == 0) return head_;
        if (index == size_ - 1) return tail_;
        Node *result = head_;
        for (int i = 0; i < index; i++)
        {
            result = result -> next;
        }
        
        return result;
    }

// РЕАЛИЗОВАНО

    LinkedList::~LinkedList() {
        Clear();
    }

    bool LinkedList::Contains(Element e) const {
        // если индекс не найден, значит и элемента нет
        return IndexOf(e) == kNotFoundElementIndex;
    }

    int LinkedList::GetSize() const {
        return size_;
    }

    bool LinkedList::IsEmpty() const {
        return size_ == 0;
    }

    Element LinkedList::tail() const {
        // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
        return tail_ ? tail_->data : Element::UNINITIALIZED;
    }

    Element LinkedList::head() const {
        return head_ ? head_->data : Element::UNINITIALIZED;
    }

// === RESTRICTED AREA: необходимо для тестирования ===

    LinkedList::LinkedList(const std::vector<Element> &elements) {
        assert(!elements.empty());

        size_ = elements.size();
        head_ = new Node(elements[0], nullptr);

        auto current_node = head_;

        for (int index = 1; index < static_cast<int>(elements.size()); index++) {
            current_node->next = new Node(elements[index], nullptr);
            current_node = current_node->next;
        }
        tail_ = current_node;
    }

    std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
        if (list.head_ != nullptr && list.tail_ != nullptr) {
            os << "{ ";
            for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
                os << internal::elem_to_str(current_node->data) << ", ";
            }
            os << internal::elem_to_str(list.tail_->data) << " }";
        } else {
            os << "{ nullptr }";
        }
        return os;
    }

    bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
        if (list.size_ != static_cast<int>(elements.size())) return false;
        Node *current_node = list.head_;

        for (const auto e : elements) {
            if (current_node == nullptr) return false;
            if (current_node->data != e) return false;
            current_node = current_node->next;
        }
        return true;
    }

}  // namespace itis