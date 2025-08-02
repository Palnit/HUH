library(dplyr)
library(ggplot2)
library(patchwork)
library(tidyr)
library(purrr)

plot_radial <- function(data, title, limit) {
  return(data |>
           ggplot(aes(x = 0, xend = x, y = y, yend = y, color = col)) +
           geom_hline(yintercept = c(1:2), linewidth = 14, color = "#dfdfdf") +
           geom_hline(yintercept = c(1:2), linewidth = 13, color = "#f7f7f7") +
           geom_segment(color = "#bf2c23", linewidth = 14.3, lineend = 'round') +
           geom_segment(linewidth = 13, lineend = 'round') +
           scale_color_identity() +
           geom_point(aes(x = x - 0.025 * y), size = 7, color = "#7f323e",
                      shape = 21, fill = 'white', stroke = 1) +
           geom_point(aes(x = x - 0.025 * y), size = 3, color = "#7f323e",
                      shape = 21, fill = 'white', stroke = 1) +
           geom_label(aes(0,
                          label = paste(type, ": ", x, time_unit, sep = "")), color = "black") +
           scale_y_continuous(limits = c(-0.6, 3.5)) +
           scale_x_continuous(limits = c(0, limit)) +
           coord_polar() +
           theme_void() +
           theme(plot.title = element_text(hjust = 0.5)) +
           ggtitle(title))
}

read.huh <- function(file, skip, chunk, keep_rows) {
  keep_rows <- c("type", "function_name", "number") %>% append(keep_rows)
  data <- read.csv(file, skip = skip, header = T)
  data <- data[!sapply(data, function(k) all(is.na(k)))]
  data <- data %>% extract(col = name, into = c("type", "function_name", "number"), regex = ".*?<(.*?)>/(.*?)/(.*)", convert = T)
  data <- data[keep_rows]
  n <- nrow(data)
  r <- rep(1:ceiling(n / chunk), each = chunk)[1:n]
  return(split(data, r))
}


scalar_data <- read.huh("/home/palnit/Desktop/code/HUHPlayground/cmake-build-debug/lib/HUH/modules/Math/benchmarks/Scalar.csv", 10, 16, c("real_time", "time_unit"))
simd_data <- read.huh("/home/palnit/Desktop/code/HUHPlayground/cmake-build-debug/lib/HUH/modules/Math/benchmarks/Simd.csv", 10, 16, c("real_time", "time_unit"))

for (i in 1:length(scalar_data)) {

  scalar_tmp <- data.frame(scalar_data[i])
  colnames(scalar_tmp) <- c("type", "function_name", "number", "real_time", "time_unit")
  simd_tmp <- data.frame(simd_data[i])
  colnames(simd_tmp) <- c("type", "function_name", "number", "real_time", "time_unit")
  combined <- as_tibble(map2(scalar_tmp, simd_tmp, ~map(transpose(list(.x, .y)), unlist)))

  plot <- do.call(patchwork::wrap_plots, pmap(list(combined$real_time, combined$number, combined$time_unit), function(tmp_data, tmp_number, tmp_unit) {
    tmp_x <- unlist(tmp_data)
    print(tmp_x)
    pow <- (floor(log10(max(tmp_x))) + 1)
    limit <- 10^pow + 2 * (10^(pow - 1))
    data <- data.frame(x = tmp_x, y = c(1:2),
                       col = c("#fbc3a0", "#eb6e49"), type = c("scalar", "simd"), time_unit = tmp_unit)
    plot_radial(data, paste("Function run:", tmp_number, "times"), limit)
  }))
  plot <- plot + plot_annotation(
    title = simd_tmp$type,
    subtitle = paste("Function run of: ", simd_tmp$function_name, " from ", simd_tmp$number[1], " to ", simd_tmp$number[length(simd_tmp$number)], " number of times with a step multiplication size of 2", sep = '')
  )
  ggsave(paste("plots/",simd_tmp$type[1], "/", simd_tmp$function_name[1], ".png", sep = ""), plot, units = "px", width = 1500, height = 1500, create.dir = T, dpi = "screen")

}
  

