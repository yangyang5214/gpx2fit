#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <time.h>

#include <math.h>

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("No file name given!\n");
        return 1;
    }

    printf("process file %s\n", argv[1]);

    // Load file
    xmlDocPtr doc = xmlParseFile(argv[1]);
    if (doc == NULL) {
        // 解析失败
        fprintf(stderr, "Failed to parse document\n");
        return -1;
    }

    xmlNodePtr cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr, "Empty document\n");
        xmlFreeDoc(doc);
        return -1;
    }

    for (cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next) {
        if (xmlStrcmp(cur->name, (const xmlChar *) "trk") == 0) {
            xmlNodePtr trkseg = cur->xmlChildrenNode;
            // 找到 <trkseg> 节点
            while (trkseg != NULL && xmlStrcmp(trkseg->name, (const xmlChar *) "trkseg") != 0) {
                trkseg = trkseg->next;
            }
            if (trkseg == NULL) {
                fprintf(stderr, "<trkseg> node not found\n");
                break;
            }
            // 遍历 <trkseg> 下的所有 <trkpt> 节点
            for (xmlNodePtr trkpt = trkseg->xmlChildrenNode; trkpt != NULL; trkpt = trkpt->next) {
                if (xmlStrcmp(trkpt->name, (const xmlChar *) "trkpt") == 0) {
                    // 提取经纬度信息
                    xmlChar *lat = xmlGetProp(trkpt, (const xmlChar *) "lat");
                    xmlChar *lon = xmlGetProp(trkpt, (const xmlChar *) "lon");
                    xmlNodePtr child = trkpt->xmlChildrenNode;
                    // 遍历 <trkpt> 下的所有子节点
                    while (child != NULL) {
                        if (xmlStrcmp(child->name, (const xmlChar *) "ele") == 0) {
                            xmlChar *ele = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
                            printf("Latitude: %s, Longitude: %s, Elevation: %s\n", lat, lon, ele);
                            xmlFree(ele);
                        } else if (xmlStrcmp(child->name, (const xmlChar *) "time") == 0) {
                            xmlChar *time = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
                            printf("Time: %s\n", time);
                            xmlFree(time);
                        }
                        child = child->next;
                    }
                    xmlFree(lat);
                    xmlFree(lon);
                }
            }
        }
    }

    // 释放资源
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}


