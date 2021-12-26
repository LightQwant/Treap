#include <utility>
#include <vector>
#include <memory>

template<typename T>
class Treap {
public:
    Treap(T value, int priority, std::shared_ptr<Treap> left, std::shared_ptr<Treap> right) :
            left_(std::move(left)), right_(std::move(right)),
            value_(value), priority_(priority) {
        size_ = 1;

        size_ += left_ != nullptr ? left_->size_ : 0;
        size_ += right_ != nullptr ? right_->size_ : 0;
    }

    [[nodiscard]] const std::shared_ptr<Treap> &Left() const {
        return left_;
    }

    [[nodiscard]] const std::shared_ptr<Treap> &Right() const {
        return right_;
    }

    T GetValue() const {
        return value_;
    }

    std::pair<std::shared_ptr<Treap>, std::shared_ptr<Treap>> Split(int key) {
        int left_size = left_ != nullptr ? left_->size_ : 0;

        std::shared_ptr<Treap> left_part;
        std::shared_ptr<Treap> right_part;

        if (left_size + 1 <= key) {
            left_part = std::make_shared<Treap>(value_, priority_, left_, nullptr);
            if (right_ != nullptr) {
                auto[left, right] = right_->Split(key - left_size - 1);
                left_part->right_ = left;
                left_part->size_ += left != nullptr ? left->size_ : 0;
                right_part = right;
            } else {
                right_part = nullptr;
            }
        } else {
            right_part = std::make_shared<Treap>(value_, priority_, nullptr, right_);
            if (left_ != nullptr) {
                auto[left, right] = left_->Split(key);
                right_part->left_ = right;
                right_part->size_ += right != nullptr ? right->size_ : 0;
                left_part = left;
            } else {
                left_part = nullptr;
            }
        }

        return {left_part, right_part};
    }

    static std::shared_ptr<Treap> Merge(const std::shared_ptr<Treap> &left,
                                        const std::shared_ptr<Treap> &right) {
        if (left == nullptr) {
            return right;
        }
        if (right == nullptr) {
            return left;
        }

        if (left->priority_ > right->priority_) {
            std::shared_ptr<Treap> new_right = Merge(left->right_, right);
            return std::make_shared<Treap>(left->value_, left->priority_,
                                           left->left_, new_right);
        } else {
            std::shared_ptr<Treap> new_left = Merge(left, right->left_);
            return std::make_shared<Treap>(right->value_, right->priority_,
                                           new_left, right->right_);
        }
    }


private:
    std::shared_ptr<Treap> left_;
    std::shared_ptr<Treap> right_;

    T value_;
    int priority_;
    int size_;
};