#ifndef RST_H
#define RST_H

void make_rst (struct tm * tp);
extern time_t (*o_mktime) (struct tm * tp);
extern struct tm * (*o_localtime_r) (const time_t * t, struct tm * tp);
extern struct tm * (*o_localtime) (const time_t * t);

#endif /* RST_H */
