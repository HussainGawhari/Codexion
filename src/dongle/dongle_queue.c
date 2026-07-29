#include "codexion.h"

void	get_dongle_queue(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_idx <= coder->right_idx)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}
