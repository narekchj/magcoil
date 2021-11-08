#include <vector>
#include <random>
#include <thread>
#include <iostream>
#include <future>
#include <algorithm>
#include <iterator>

class Calc
{
  public:
    Calc()
      : hardNum(4 * std::thread::hardware_concurrency())
      {}

    void populate()
    {
      mData.reserve(1000);

      std::cout << "Available threads " << hardNum << std::endl;
      std::future<void> f[hardNum];
      for(auto i = 0u; i < hardNum; ++i)
      {
        f[i] = std::async(std::launch::async, &Calc::doJob, this);
      }
    }

    auto& get() { return mData; }

  private:
    void doJob()
    {
      while(true)
      {
        //doing something
        {
          std::lock_guard<std::mutex> lock(mMutex);
          if (mData.size() >= (1000 - mInProgressCnt)) return;
          ++mInProgressCnt;
        }

        auto val = getRandomNum();
        {
          using namespace std::chrono;
          std::this_thread::sleep_for(10ms);
        }

        {
          std::lock_guard<std::mutex> lock(mMutex);
          mData.push_back(val);
          --mInProgressCnt;
        }
      }
    }

  private:
    static int getRandomNum()
    {
      static std::random_device rd;
      static std::uniform_int_distribution<int> dst(0,100);

      return dst(rd);
    }

    const unsigned hardNum;

  private:
    std::vector<int> mData;
    std::mutex mMutex;

    size_t mInProgressCnt = 0;
};

int main()
{
  Calc c;

  c.populate();

  std::cout << c.get().size() << std::endl;
  //std::sort(c.get().begin(), c.get().end(), std::less<int>());
  //std::transform(c.get().begin(), c.get().end(), std::ostream_iterator<int>(std::cout,"\n"), [](const auto& v) {return v;});

  return 0;
}
