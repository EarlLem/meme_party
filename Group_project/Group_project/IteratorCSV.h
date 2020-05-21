#include"File_identifier.h"//не знаем, как лидин файл называется


vector<string> mapnot(std::map<std::string, algorithm*> map) {
    std::vector<string> vs;
    vs.reserve(map.size());
    for (auto const& imap : map)
        vs.push_back(imap.first);
    return vs;
}
//вычленили пути из мапа в вектор стрингов

template <typename T, typename U>
class Iterator {
public:
    typedef typename std::vector<T>::iterator iter_type;
    Iterator(U* p_data, bool reverse = false) : m_p_data_(p_data) {
        m_it_ = m_p_data_->m_data_.begin();
    }
    void First() {
        m_it_ = m_p_data_->m_data_.begin();
    }
    void Next() { m_it_++; }

    bool IsDone() {
        return (m_it_ == m_p_data_->m_data_.end());
    }

    int Current() { return m_it_; }

private:
    U* m_p_data_;
};


template <class T>
class Container {
    friend class Iterator<T, Container>;

public:
    void Add(T a) {
        m_data_.push_back(a);
    }

    Iterator<T, Container>* CreateIterator() {
        return new Iterator<T, Container>(this);
    }

    T operator[](size_t index) {
        return m_data_[index];
    }

private:
    std::vector<T> m_data_;
};

//подается лидин мэп, в котором ключи это пути к файлам, значения это методы

//возвращается так же мэп, в котором ключи те же, но значения это ссылки на матрицы
map<string, Matrix<double>&> ClientCode(std::map<std::string, algorithm*> map) {
    vector<string> v = mapnot(map);//сохраняем пути в новый вектор
    Container<string> cont;

    for (int i; i < v.size(); i++) {
        cont.Add(v[i]);
    }

    std::map<std::string, Matrix<double>&> matrmap;//создаем пустой мэп, который возвратимì

    Iterator<std::string, Container<std::string>>* it = cont.CreateIterator();
    for (it->First(); !it->IsDone(); it->Next()) {
        matrmap.insert(std::pair<std::string, Matrix<double>&>(cont[it->Current()], CSVtoMatrix<double>(cont[it->Current()])));//обрабатываем файл
    }

    return matrmap;
}
