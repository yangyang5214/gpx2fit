#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gpxp.h"
#include <libxml/tree.h>

struct Point *createPointArray(int size) {
    return (struct Point *) malloc(size * sizeof(struct Point));
}

long seconds_since_fit_epoch(const char *time_str) {
    //1989-12-31T:00:00:00Z -> UTC + 631036800
    struct tm tm_time;
    strptime(time_str, "%Y-%m-%dT%H:%M:%S", &tm_time);
    return (mktime(&tm_time) - 631036800);
}

struct Point *parserGpx(const char *filename, int *numPoints) {
    xmlDocPtr doc = xmlParseFile(filename);
    if (doc == NULL) {
        // 解析失败
        fprintf(stderr, "Failed to parse document\n");
        return NULL;
    }

    xmlNodePtr cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr, "Empty document\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    int count = 0;
    struct Point *points = createPointArray(100); // 初始分配100个元素的数组

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

                    struct Point point;

                    xmlChar *lat = xmlGetProp(trkpt, (const xmlChar *) "lat");
                    xmlChar *lon = xmlGetProp(trkpt, (const xmlChar *) "lon");

                    point.lat = strtod((const char *) lat, NULL);
                    point.lon = strtod((const char *) lon, NULL);

                    xmlFree(lat);
                    xmlFree(lon);


                    xmlNodePtr child = trkpt->xmlChildrenNode;
                    // 遍历 <trkpt> 下的所有子节点
                    while (child != NULL) {
                        if (xmlStrcmp(child->name, (const xmlChar *) "ele") == 0) {
                            xmlChar *ele = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
                            point.ele = strtod((const char *) ele, NULL);
                            xmlFree(ele);
                        } else if (xmlStrcmp(child->name, (const xmlChar *) "time") == 0) {
                            xmlChar *time = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
                            point.time_ts = seconds_since_fit_epoch((char *) time);
                            xmlFree(time);
                        }
                        child = child->next;
                    }

                    points[count++] = point;
                    if (count % 100 == 0) {
                        points = realloc(points, (count + 100) * sizeof(struct Point));
                    }
                }
            }
        }
    }
    xmlFreeDoc(doc);
    xmlCleanupParser();

    *numPoints = count;
    return points;
}


