#ifndef RESOURCEMANAGE_H
#define RESOURCEMANAGE_H
/*************************************************************************
* function：资源管理器定义文件，
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include <map>
#include <list>
#include <mutex>

//
namespace mqw
{
    template <typename T>
    class ResourceManage
    {
    private:
        // 记录所有的资源个数
        std::size_t count_all_ = 0;
        // 每个内存块上的资源个数
        std::size_t count_per_unit_ = 10;
        // 正在使用中的资源列表
        std::map<T*, T*> using_list_;
        // 未使用的资源列表
        std::list<T*> unused_list_;
        // 资源块列表
        std::list<T*> resource_block_ptr_;
        // 此资源表存储了非资源块的分配资源
        std::list<T*> backup_list_;
        // 锁
        std::recursive_mutex list_lock_;
        // 限制内存分配，初始化后设置.默认不限制
        bool is_limited_ = false;
        // 限制的倍数，即初始化区块大小的倍数
        std::size_t count_times_ = 1;

    public:
        ResourceManage() = delete;
        ResourceManage(std::size_t init_count);
        ~ResourceManage();

        bool reinitialize(std::size_t reinit_count);
        void set_limit(bool is_limit, std::size_t times);
        void reset();
        T *get();
        bool put(T *rsc);
        inline std::size_t unused_size();
        inline std::size_t max_size();
        inline std::size_t unit_size();
        inline bool available();

    private:
        void _clear();
    };

    template <typename T>
    ResourceManage<T>::ResourceManage(std::size_t init_count)
        :count_all_(init_count), count_per_unit_(init_count)
    {
        if (init_count > 0)
        {
            T* rsc_block = new T[count_per_unit_];
            for (std::size_t i = 0; i < count_per_unit_; i++)
            {
                unused_list_.push_back(rsc_block + i);
            }
            resource_block_ptr_.push_back(rsc_block);
        }
    }

    template <typename T>
    ResourceManage<T>::~ResourceManage()
    {
        _clear();
    }

    template <typename T>
    bool ResourceManage<T>::reinitialize(std::size_t reinit_count)
    {
        if (reinit_count == 0)
            return false;
        if (resource_block_ptr_.size() > 0)
        {
            _clear();
        }
        count_all_ = reinit_count;
        count_per_unit_ = reinit_count;
        try{
            T* rsc_block = new T[count_per_unit_];
            for (std::size_t i = 0; i < count_per_unit_; i++)
            {
                unused_list_.push_back(rsc_block + i);
            }
            resource_block_ptr_.push_back(rsc_block);
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    template <typename T>
    void ResourceManage<T>::set_limit(bool is_limit, std::size_t times)
    {
        is_limited_ = is_limit;
        count_times_ = times >= 1 ? times : 1;
    }

    template <typename T>
    void ResourceManage<T>::reset()
    {
        std::lock_guard<std::recursive_mutex> lock(list_lock_);
        for (auto iter : using_list_)
        {
            unused_list_.push_back(iter.second);
        }
        using_list_.clear();
    }

    template<typename T>
    T *ResourceManage<T>::get()
    {
        std::lock_guard<std::recursive_mutex> lock(list_lock_);
        if (unused_list_.size() <= 0)
        {
            if (is_limited_ && resource_block_ptr_.size() >= count_times_)
            {
                return nullptr;
            }

            try{
                T *new_block = new T[count_per_unit_];
                for (std::size_t i = 0; i < count_per_unit_; i++)
                {
                    unused_list_.push_back(new_block + i);
                }
                resource_block_ptr_.push_back(new_block);
                count_all_ += count_per_unit_;
            }
            catch (...)
            {
                return nullptr;
            }
        }

        T *tmp_user = unused_list_.front();
        unused_list_.pop_front();
        using_list_[tmp_user] = tmp_user;
        return tmp_user;
    }

    template<typename T>
    bool ResourceManage<T>::put(T *user)
    {
        std::lock_guard<std::recursive_mutex> lock(list_lock_);
        if (NULL == using_list_[user])
        {
            return false;
        }
        using_list_.erase(user);
        unused_list_.push_back(user);
        return true;
    }

    template<typename T>
    std::size_t ResourceManage<T>::unused_size()
    {
        return unused_list_.size();
    }

    template<typename T>
    std::size_t ResourceManage<T>::max_size()
    {
        return count_all_;
    }

    template<typename T>
    std::size_t ResourceManage<T>::unit_size()
    {
        return count_per_unit_;
    }

    template<typename T>
    bool ResourceManage<T>::available()
    {
        if (count_all_ == 0)
            return false;
        if (resource_block_ptr_.size() * count_per_unit_ != count_all_)
            return false;
        return true;
    }

    template<typename T>
    void ResourceManage<T>::_clear()
    {
        for (auto iter : backup_list_)
        {
            if (iter)
                delete iter;
        }
        for (auto iter : resource_block_ptr_)
        {
            delete[] iter;
        }
        resource_block_ptr_.clear();
        backup_list_.clear();
        using_list_.clear();
        unused_list_.clear();
    }
}

#endif //RESOURCEMANAGE_H