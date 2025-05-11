/* File: csv_writer.h */
#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "ast.h"

void generate_csv_from_ast(ASTNode* root, const char* out_dir);

#endif
//     write_csv_row(f, global_id++, values, count);
//   }
