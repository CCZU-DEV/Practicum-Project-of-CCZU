#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ������ʼ����ɺ�ķ���ֵ */
const int success = 1;
const int failed = 0;

/* ��������С���� */
#define buffer_size 1024

/* �洢������Ϣ�Ľṹ�壬��ϸ�������Բ鿴 https://github.com/patorjk/figlet.js/blob/master/doc/figfont.txt */
typedef struct tagFontConfig {
	char hardblank;			// ǿ�ƿո�
	int height;				// ����߶�
	int baseline;			// ��׼�ߣ�û�õ���
	int max_length;			// �����
	int old_layout;			// �Ƿ�Ϊ�ɲ��֣�û�õ���
	int comment_lines;		// ע������
	int print_direction;	// �������û�õ���
	int full_layout;		// �������֣�δʹ�ã�
	int codeag_count;

	char **font_file;		// ���д洢��������
	int font_file_lines;	// ����������������������ڴ�
} fontcfg_t;

/* �洢һ���ֵĽṹ�� */
typedef struct tagLine {
	char **lines;			// ʵ���ϵ�һ��
	int height;				// ����߶�
	int width;				// ��һ���ֵĿ��
} line_t;

/* ���ֶ������� */
typedef enum { left, center, right } align_t;

const char *defaultFont = "3d_diagonal.flf";	// Ĭ������
const char *signature = "flf2a";				// ǩ�����ο����������

/*
 * ��������
 * font_path Ϊ����·����
 * cfg Ϊ������Ϻ�洢��Ϣ�Ľṹ��
 */
int load_font(const char *font_path, fontcfg_t *cfg) {
	char header[buffer_size], buffer[buffer_size];
	int i;
	int max_line, readed;
	FILE *font = NULL;

	// ���ļ�
	font = fopen(font_path, "r");
	if (!font)
		return failed;

	// ��ȫ�����ݶ����ڴ棬����������� realloc ������չ font_file ���ֵ��ڴ�
	for (max_line = 20, readed = 0, cfg->font_file = malloc(sizeof(char *) * 20); !feof(font); ++readed) {
		if (max_line == readed) {
			max_line += 20;
			cfg->font_file = (char **)realloc(cfg->font_file, max_line * sizeof(char *));
		}
		fgets(buffer, buffer_size, font);
		cfg->font_file[readed] = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(cfg->font_file[readed], buffer);
	}
	// ����������
	cfg->font_file_lines = readed;

	// ��ȡ header �����ǩ��
	strcpy(header, cfg->font_file[0]);
	for (i = 0; i != strlen(signature); ++i)
		if (signature[i] != header[i]) {
			return failed;
		}
	// ��ȡ������Ϣ
	sscanf(header, "flf2a%c %d %d %d %d %d", // %d %d %d",
		&cfg->hardblank,
		&cfg->height,
		&cfg->baseline,
		&cfg->max_length,
		&cfg->old_layout,
		&cfg->comment_lines/*, cfg->print_direction, cfg->full_layout, cfg->codeag_count*/
	);
	return success;
}

/*
 * ��Ⱦһ���֣����� cfg �����������ʾ string �е��ַ�
 */
line_t renderWord(fontcfg_t *cfg, const char *string) {
	line_t line;
	int i, j, offist;

	line.height = cfg->height;
	line.width = 0;

	// ��ʼ��ÿһ�е���Ϣ
	line.lines = (char **)malloc(line.height * sizeof(char *));
	for (i = 0; i != line.height; ++i) {
		line.lines[i] = (char *)malloc(cfg->max_length * strlen(string));
		line.lines[i][0] = '\0';
	}

	// �������Ⱦ��ÿ�ж���Ⱦ������������
	// ��Ⱦ����ַ������ max_length �洢�����Ƶ��ṹ����
	for (offist = i = 0; i != strlen(string); ++i) {
		int max_length = 0;
		for (j = 0; j != line.height; ++j) {
			if (string[i] - ' ' < 0)
				continue;
			strcat(line.lines[j], cfg->font_file[cfg->comment_lines + (string[i] - ' ') * cfg->height + j + 1]);
			line.lines[j][strlen(line.lines[j]) - 3 - (j == line.height - 1 ? 1 : 0)] = '\0';
			max_length = max(max_length, strlen(line.lines[j]));
		}
		line.width = max(max_length, line.width);
	}

	// ��Ӳ�ո�ԭΪ��ͨ�ַ��Ŀո�
	for (i = 0; i != line.height; ++i) {
		for (j = 0; line.lines[i][j]; ++j) {
			if (line.lines[i][j] == cfg->hardblank) {
				line.lines[i][j] = ' ';
			}
		}
	}
	return line;
}

/*
* ����������
* ��� line1 Ϊ�գ����� line1 �д���һ�� line2 �ĸ���
*/
void linecat(line_t *line1, line_t *line2) {
	int i;
	if (line1->height != line2->height) {
		// line1 Ϊ�գ�����
		line1->height = line2->height;
		line1->lines = (char **)malloc(line2->height * sizeof(char *));
		for (i = 0; i != line2->height; ++i) {
			line1->lines[i] = malloc((strlen(line2->lines[i]) + 2) * sizeof(char));
			strcpy(line1->lines[i], line2->lines[i]);
			line1->width = line2->width;
		}
		return;
	}
	// line1 �ǿգ�����
	for (i = 0; i != line1->height; ++i) {
		int pos = strlen(line1->lines[i]);
		line1->lines[i] = realloc(line1->lines[i], (strlen(line1->lines[i]) + strlen(line2->lines[i]) + 2) * sizeof(char));
		line1->lines[i][pos] = '\0';
		strcat(line1->lines[i], line2->lines[i]);
		line1->width = max(line1->width, strlen(line1->lines[i]));
	}
}

// �ͷ�������Ϣ��ռ�õ���Դ
void free_font(fontcfg_t *cfg) {
	int i;
	for (i = 0; i != cfg->font_file_lines; ++i)
		free(cfg->font_file[i]);
	free(cfg->font_file);
}

// �ͷ�һ������ռ�õ���Դ
void free_line(line_t line) {
	int i;
	if (line.width == 0) return;
	for (i = 0; i != line.height; ++i)
		free(line.lines[i]);
	free(line.lines);
}

// ��� cnt ���ո�
void print_space(int cnt) {
	while (cnt--) putchar(' ');
}

// �� line �е����ݰ��ն��뷽ʽ align ���ֵ��Ļ������������ target_width Լ��
void print_line(align_t align, int target_width, line_t line) {
	int spaces = 0, i;
	switch (align) {
	default:
	case left:
		break;
	case right:
		spaces = target_width - line.width - 1;
		break;
	case center:
		spaces = (target_width - line.width - 1) / 2;
		break;
	}
	for (i = 0; i != line.height; ++i) {
		print_space(spaces);
		puts(line.lines[i]);
	}
}

int main(int argc, char *argv[]) {
	fontcfg_t fontcfg;
	int target_width = 80;
	align_t align;
	int i;
	char in[buffer_size];
	char *font = defaultFont;

	// �������е������в���
	for (i = 1; i != argc; ++i) {
		if (strcmp(argv[i], "-c") == 0) {		// ���뷽ʽ������
			align = center;
		}
		else if (strcmp(argv[i], "-l") == 0) {	// ���뷽ʽ������
			align = left;
		}
		else if (strcmp(argv[i], "-r") == 0) {	// ���뷽ʽ������
			align = right;
		}
		else if (strcmp(argv[i], "-w") == 0) {	// ָ��������
			i++;
			sscanf(argv[i], "%d", &target_width);
		}
		else if (strcmp(argv[i], "-f") == 0) {	// ָ�������ļ� 
			// ���ڽ�ѹzip ��������ʵ�֣���Ҫ��ǰ��winrar֮��Ĺ��߽������ѹ��
			// �������� 3-d.flf������Ҫ�� 3-d.flf ������Ϊ 3-d.zip ���򿪣��������3-d.f
			// lf ��ѹ������/��ִ���ļ�Ŀ¼������ʹ��
			i++;
			font = argv[i];
		}
	}

	// ������������
	if (!load_font(font, &fontcfg)) {
		printf("Cannot load font file.");
		return EXIT_FAILURE;
	}

	for (;;) {	// ��ѭ����������ȡ����ֱ���û����� quit
		line_t line, word, ch;

		fgets(in, buffer_size, stdin);
		in[strlen(in) - 1] = '\0';
		line.lines = word.lines = NULL;
		line.width = word.width = 0;
		line.height = word.height = 0;

		for (i = 0; i != strlen(in); ++i) {
			char str[20];
			str[0] = in[i], str[1] = '\0';
			ch = renderWord(&fontcfg, str);
			/*
			 * ��Ⱦ˼·��
			 *     ÿ����Ⱦһ���ַ���Ȼ���齫����ַ��͵������������Ƿ�ᳬ��ָ���ķ�Χ��
			 * ����ǣ����Ƚ�����Ⱦ�ĵ������ݴ�ӡ����Ļ������ֹ����ĸ�м��������������˳
			 * ����Ⱦ��һ�����ʣ��ͼ�������������ʷŵ��������Ƿ�ᳬ������������������
			 * ���е����ݣ�����ͽ�����������ӵ�����ĩβ����׼����һ�����ʡ�
			 *     ���в��ѿ��������������ʳ�����Ļ��Χʱ����Ҫ��ǰ���ѷ����ھ����е�������
			 * ������Ļ������֤˳�򲻳��ִ���
			 */
			if (word.width + ch.width >= target_width - 1) {	// �������ַ��͵������������Ƿ�ᳬ����Χ
				if (line.width != 0) {							// �Ѿ��о���û�������Ҫ�ȴ���
					print_line(align, target_width, line);
					line.width = line.height = 0;
				}
				print_line(align, target_width, word);
				free_line(word);
				word.width = word.height = 0;
			}
			linecat(&word, &ch);	// ����ĸ���ӵ�����ĩβ

			if (in[i] == ' ' && word.width > 1) {	// �ո���ǵ��ʵĽ�����������ӵ���β�᲻�ᳬ��
				if (line.width + word.width >= target_width - 1) {
					print_line(align, target_width, line);
					line.width = line.height = 0;
				}
				linecat(&line, &word);
				free_line(word);
				word.width = word.height = 0;
			}
		}
		// �������ڻ���ĵ��Ӻ;����е��������
		if (line.height && line.width + word.width >= target_width - 1) {
			print_line(align, target_width, line);
			line.width = line.height = 0;
		}
		linecat(&line, &word);
		word.width = word.height = 0;
		print_line(align, target_width, line);
		// ����Ƿ�Ҫ�˳�
		if (strcmp(in, "quit") == 0) break;
	}
	// �ͷ��������Դ�����˳�
	free_font(&fontcfg);
	return EXIT_SUCCESS;
}