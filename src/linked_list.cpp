#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

    void LinkedList::Add(Element e) {
        auto node = new Node(e, nullptr);
        if (size_ == 0) {
            head_ = node;
            tail_ = head_;
        } else {
            tail_ -> next = node;
            tail_ = node;
        }
        size_++;
    }

    void LinkedList::Insert(int index, Element e) {
        internal::check_out_of_range(index, 0, size_ + 1);
        if (size_ == 0) {
            head_ = new Node(e, nullptr);
            tail_ = head_;
        } else if (index == 0) {
            Node *node = new Node(e, head_);
            head_ = node;
        } else if (index == size_) {
            Add(e);
            return;
        } else {
            Node *n = find_node(index);
            auto new_Node = new Node(e, n -> next);
            n -> next = new_Node;
        }
        size_++;
    }

    void LinkedList::Set(int index, Element e) {
        internal::check_out_of_range(index, 0, size_);
        find_node(index) -> data = e;
    }

    Element LinkedList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        Node *nodeToDelete;
        if (index == 0) {
            nodeToDelete = head_;
            head_ = head_ -> next;
        } else {
            Node *prev_Node = find_node(index - 1);
            nodeToDelete = prev_Node -> next;
            prev_Node -> next = nodeToDelete -> next;
        }
        size_--;
        Element e = nodeToDelete -> data;
        delete nodeToDelete;
        return e;
    }

    void LinkedList::Clear() {
        // Tip 1: люди в черном (MIB) пришли стереть вам память
        // напишите свой код здесь ...
        Node *current = head_;
        Node *next;
        while (current != nullptr)
        {
            next = current -> next;
            delete current;
            current = next;
        }
        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    Element LinkedList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);
        return find_node(index) -> data;
    }

    int LinkedList::IndexOf(Element e) const {
        // напишите свой код здесь ...
        Node *result = head_;
        int i = 0;
        while (i < size_ && result -> data != e) {
            result = result -> next;
            i++;
        }
        return i == size_ ? kNotFoundElementIndex : i;
    }

    Node *LinkedList::find_node(int index) const {
        assert(index >= 0 && index < size_);
        // Tip 1: можете сразу обработать случаи поиска начала и конца списка
        // напишите свой код здесь ...
        if (index == 0) return head_;
        if (index == size_ - 1) return tail_;
        Node *result = head_;
        while (index > 0) {
            result = result -> next;
            index--;
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