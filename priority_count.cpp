#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string.h>


typedef struct 
{
    int64_t count;
    char stem[64];
} count_node;

bool compare_nodes(const count_node & lhs, const count_node & rhs)
{
    return lhs.count < rhs.count;
}

int main(int argc, char * argv[])
{
    FILE * fp = NULL;

    if(argc == 3)
    {
        fp = fopen(argv[1],"r");
        if(fp)
        {
            std::vector<count_node> node_list;

            while(1)
            {
                char buf[1024] = {0};
                if(NULL != fgets(buf,sizeof(buf),fp))
                {
                    count_node new_node;

                    if(2 == sscanf(buf,"%lld|%s\n",&new_node.count, new_node.stem))
                    {
                        node_list.push_back(new_node);
                        fprintf(stdout,"%lld|%s\n",new_node.count, new_node.stem);
                    }
                }
                else
                {
                    break;
                }
                if(node_list.size() > 100000)
                {
                    break;
                }
            }
            std::make_heap(node_list.begin(),node_list.end(),compare_nodes);

            for(size_t i=0;i<1000;i++)
            {
                fprintf(stdout,"%lld|%s\n",node_list[0].count, node_list[0].stem);
                std::pop_heap(node_list.begin(),node_list.end(),compare_nodes);
                node_list.resize(node_list.size()-1);

            }
            fclose(fp);
            fp = NULL;
        }
    }
}
