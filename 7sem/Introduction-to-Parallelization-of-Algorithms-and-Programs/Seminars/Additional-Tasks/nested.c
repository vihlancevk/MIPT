#include <omp.h>
#include <stdio.h>

void report() {
	int position = 0;
	char buffer[1024];

	int cur_thread = omp_get_thread_num();
	position += snprintf(
					buffer + position,
					64,
					"Current thread: %d.\n",
					cur_thread
				);

	int num_active_levels = omp_get_active_level();
	position += snprintf(
					buffer + position,
					64,
					"Number active levels: %d.\n",
					num_active_levels
				);

	int level = omp_get_level();
	int total_number_of_threads_at_level = 1;
	for (int i = 1; i <= level; ++i) {
		position += snprintf(buffer + position, 32, "Level %d: ", i);

		total_number_of_threads_at_level *= omp_get_team_size(i);
		position += snprintf(
			buffer + position,
			64,
			"total number of threads at level: - %d.\n",
            total_number_of_threads_at_level
        );
	}

	printf("%s", buffer);
}

int main() {
	int number_of_nestings = 3;
	omp_set_nested(number_of_nestings);	

    omp_set_dynamic(0);
    #pragma omp parallel num_threads(2)
    {
		report();
        
		#pragma omp parallel num_threads(2)
        {
        	report();
            
			#pragma omp parallel num_threads(2)
            {
        		report();
            }
        }
    }
    
	return(0);
}
