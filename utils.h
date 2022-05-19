#ifndef UTILS_H_
#define UTILS_H_

#define DIE(assertion, call_description)	\
	do {									\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",	\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(EXIT_FAILURE);				\
		}									\
	} while (0)

#define min(a, b) 							\
	({ __typeof__(a) _a = (a);				\
		__typeof__(b) _b = (b); 			\
		_a > _b ? _b : _a; })

#endif  // UTILS_H_