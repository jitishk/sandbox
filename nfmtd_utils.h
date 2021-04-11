#ifndef NFMTD_UTILS_H
#define NFMTD_UTILS_H

void nfmtd_init();

#define nfmtd_mem_dump(id, type, data, size) nfmtd_mem_dump_hex(id, ((const uint8_t *)(type)), ((const char *)data), (size)) 

void nfmtd_mem_dump_hex(uint64_t id, const char *type, const uint8_t * pkt_in, size_t pkt_size);

#endif /* end of include guard: NFMTD_UTILS_H */
