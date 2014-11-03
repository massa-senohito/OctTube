
template<typename T>
class DotnetList
{
#ifdef _MANAGED
  List<T> data;
#else
  typedef std::vector<T> TS;
  //typedef std::vector<T>::iterator TSI;
  //template<typename T>
#endif
public:
  DotnetList<T>();
  TS* Data;
  ~DotnetList<T>();
  void Add(T);
  //TSI First();
  //TSI Last();
  void Clear();
  int Count;

  int Accumlate(std::function<int(int,T)>);
  void ForEach(std::function<void(T)>);
  T operator[](int i);
};

template<typename T>
DotnetList<T>::DotnetList() :Count(0)
{
  Data = new TS();
}
template<typename T>
T Add(T x, T y){
  return x + y;
}
template<typename T>
int DotnetList<T>::Accumlate(std::function<int(int,T)> f)
  
{
  typedef decltype(*Data->begin()) IT;
  int init = int();
  auto first = Data->begin();
  for (; first != Data->end(); ++first){
    init = f(init, *first);
  }
  return init;
}
template<typename T>
void DotnetList<T>::ForEach(std::function<void(T)> f)
  
{
  typedef decltype(*Data->begin()) IT;
  auto first = Data->begin();
  for (; first != Data->end(); ++first){
    f(*first);
  }
}

template<typename T,typename RT>
auto Sum(std::vector<T> Data)->decltype(*Data->begin())
{
  typedef decltype(*Data->begin()) IT;
  IT init = IT();
  return std::accumulate(Data->begin(), Data->end(), init, Add);
}
template<typename T,typename RT>
auto Map(std::vector<T>& data, std::function<RT(T)> mf)->std::vector<RT>{
  return std::transform(data.begin(), data.end(), mf);
}

template<typename T>
DotnetList<T>::~DotnetList(){
  Data->clear();
  delete Data;
}

template<typename T>
void DotnetList<T>::Add(T item){
  ++Count;
  Data->push_back(item);
}
template<typename T>
void DotnetList<T>::Clear(){
  Data->clear();
  Count = 0;
}
//template<typename T>
//TSI
//  DotnetList<T>::First()
//{
//  return data->begin();
//}
//template<typename T>
//TSI
//  DotnetList<T>::Last()
//{
//  return data->end();
//}

template<typename T>
T DotnetList<T>::operator[](int i)
{
  return Data->at(i);
}
#ifdef _MANAGED
#else
#endif