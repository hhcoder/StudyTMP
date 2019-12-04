#include <iostream>
#include <vector>
#include <cstring>

// Let's say, we have following data flow
//
// YuvImg -> [MobileNetProcess] -> ListsOfRect 
//                                      |
//                                      v
//                                 [SodProcess] -> 10 Rects  -> [AecProcess] -> Gain and Exposure
//                                                                  ^
//                                                                  |
//                                                                Stats

// So we have following classes and processing functions

struct Rect
{
    int x, y, w, h;
};

struct YuvImg
{
    int w, h, s;
    uint8_t* y, uv;
};

struct MobileNetOutput
{
    Rect rect_array[10];
    int type_array[10];
    int valid_len;
};

struct SodOutput
{
    Rect rect_array[10];
    int valid_len;
};

struct StatsData 
{
    int valid_w, valid_h;
    int gr[6*4], gb[6*4], r[6*4], b[6*4];
};

struct AecOutput
{
    float gain, exposure;
};

void MobileNetProc(const YuvImg& in, MobileNetOutput& out)
{
    std::cout << "MobileNetProc" << std::endl;
}

void SodProc(const MobileNetOutput& in, SodOutput& out)
{
    std::cout << "SodProc" << std::endl;
}

void AecProc(const StatsData& in_stats, const SodOutput& in_sod, AecOutput& out)
{
    std::cout << "AecProc" << std::endl;
}

struct TraditionalMethod
{
    AecOutput aec_result;

    void Process(const YuvImg& in_img, const StatsData& in_stats)
    {
        MobileNetOutput mobile_net_result;
        MobileNetProc(in_img, mobile_net_result);

        SodOutput sod_result;
        SodProc(mobile_net_result, sod_result);

        AecProc(in_stats, sod_result, aec_result);
    }

    AecOutput GetResult()
    {
        return aec_result;
    }

    static AecOutput Execute(const YuvImg& in_img, const StatsData& in_stats)
    {
        TraditionalMethod* tm = new TraditionalMethod;

        std::cout << "=======" << "Traditional Method" << "=======" << std::endl;
        for (int i=0; i<3; i++)
        {
            std::cout << i << std::endl;
            tm->Process(in_img, in_stats);
        }
        std::cout << "=======" << "=======" << "=======" << std::endl;

        AecOutput aec_result = tm->GetResult();
        delete tm;
        return aec_result;
    }
};

struct ObjectOrientedMethod
{
    struct Base 
    {
        virtual void SetInput1(const void* in) = 0;
        virtual void SetInput2(const void* in) {};
        virtual void Process() = 0;
        virtual void* GetOutput() = 0;
    };

    class BTree
    {
    public:
        BTree() : root(nullptr) {}
        void Add(Base* ip, int iv, const void* in1, const void* in2)
        {
            Node* n = new Node(ip, iv, in1, in2);
            if(nullptr==root) root = n;
            else Add(root, n);
        }
        void Proc() { Proc(root); }
    private:
        struct Node
        {
            Node *l, *r;
            Base* proc;
            int v;
            Node(Base* ip, int iv, const void* in1, const void* in2) : l(nullptr), r(nullptr), proc(ip), v(iv) 
            { 
                if(nullptr!=in1) proc->SetInput1(in1); 
                if(nullptr!=in2) proc->SetInput2(in2);
            }
        };
        Node* root;
        void Add(Node* r, Node* n)
        {
            if(n->v > r->v)
            {
                if(nullptr==r->r) r->r = n;
                else Add(r->r, n);
            }
            else
            {
                if(nullptr==r->l) r->l = n;
                else Add(r->l, n);
            }
        }
        void Proc(Node* r)
        {
            if(nullptr==r) return;
            Proc(r->l);
            Proc(r->r);
            if(nullptr!=r->l) { r->proc->SetInput1(r->l->proc->GetOutput()); }
            if(nullptr!=r->r) { r->proc->SetInput2(r->r->proc->GetOutput()); std::cout << "here" << std::endl; }
            r->proc->Process();
            std::cout << r->v << std::endl;
        }
        // void GetResult(void* result, size_t s) { memcpy(result, root->proc->GetOutput(), s); }
    };

    struct MobileNet : public Base
    {
        typedef YuvImg In1;
        typedef MobileNetOutput Out;
        In1 in1;
        Out out;
        virtual void SetInput1(const void* in) { memcpy(&in1, in, sizeof(In1)); }
        virtual void Process(){ MobileNetProc(in1, out); }
        virtual void* GetOutput() { return static_cast<void*>(&out);  }
        virtual ~MobileNet(){}
    };

    struct Sod : public Base
    {
        typedef MobileNetOutput In1;
        typedef SodOutput Out;
        In1 in1;
        Out out;
        virtual void SetInput1(const void* in) { 
            memcpy(&in1, in, sizeof(in1)); 
        }
        virtual void Process() { SodProc(in1, out); }
        virtual void* GetOutput() { return static_cast<void*>(&out); }
        virtual ~Sod(){};
    };

    struct Aec : public Base
    {
        typedef StatsData In1;
        typedef SodOutput In2;
        typedef AecOutput Out;
        In1 in1;
        In2 in2;
        Out out;
        virtual void SetInput1(const void* in) { memcpy(&in1, in, sizeof(In1)); }
        virtual void SetInput2(const void* in) { memcpy(&in2, in, sizeof(In2)); }

        virtual void Process() { AecProc(in1, in2, out); }
        virtual void* GetOutput() { return static_cast<void*>(&out); std::cout << "Aec: " << &out << std::endl;}
        virtual ~Aec(){}
    };

    static AecOutput Execute(const YuvImg& in_img, const StatsData& in_stats)
    {
        AecOutput aec;
        BTree btree;

        btree.Add(new Aec, 4, nullptr, static_cast<const void*>(&in_stats));
        btree.Add(new Sod, 3, nullptr, nullptr);
        btree.Add(new MobileNet, 2, static_cast<const void*>(&in_img), nullptr);

        btree.Proc();
        return aec;
    }
};


int main(int argc, char* argv[])
{
    YuvImg src_yuv;
    StatsData src_stats;

    AecOutput result = TraditionalMethod::Execute(src_yuv, src_stats);

    AecOutput result2 = ObjectOrientedMethod::Execute(src_yuv, src_stats);

    return 0;
}
