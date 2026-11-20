# Plot Google Benchmark JSON output as radial wheels.
# Usage:
#   Rscript plot_google_benchmark_radial.R benchmark.json
#   Rscript plot_google_benchmark_radial.R benchmark.json output_directory
#
# Output layout:
#   <output_directory>/<json_base_name>/<function_name>/<function_name>_<combined_type>.png
#
# Each PNG contains one radial wheel per benchmark argument after the final
# slash (for example, /8 or /16). Each structure/backend gets its own ring.
# If a benchmark was run repeatedly, the median real_time is plotted.

required_packages <- c("jsonlite", "dplyr", "ggplot2", "patchwork", "purrr", "tibble")
missing_packages <- required_packages[!vapply(required_packages, requireNamespace, logical(1), quietly = TRUE)]
if (length(missing_packages) > 0) {
  stop(
    "Missing R packages: ", paste(missing_packages, collapse = ", "),
    ". Install them with install.packages(c(",
    paste(sprintf("'%s'", missing_packages), collapse = ", "), ")).",
    call. = FALSE
  )
}

suppressPackageStartupMessages({
  library(dplyr)
  library(ggplot2)
  library(patchwork)
  library(purrr)
  library(tibble)
})

# Split C++ template arguments only at commas that are not nested inside
# another template, parentheses, brackets, or braces. This also handles types
# such as std::array<int, 4> as a single argument.
split_cpp_template_args <- function(x) {
  chars <- strsplit(x, "", fixed = TRUE)[[1]]
  if (length(chars) == 0) {
    return(character())
  }

  parts <- character()
  current <- character()
  angle_depth <- 0L
  paren_depth <- 0L
  bracket_depth <- 0L
  brace_depth <- 0L

  for (ch in chars) {
    if (ch == "," && angle_depth == 0L && paren_depth == 0L &&
        bracket_depth == 0L && brace_depth == 0L) {
      parts <- c(parts, paste0(current, collapse = ""))
      current <- character()
      next
    }

    current <- c(current, ch)

    if (ch == "<") angle_depth <- angle_depth + 1L
    if (ch == ">") angle_depth <- max(0L, angle_depth - 1L)
    if (ch == "(") paren_depth <- paren_depth + 1L
    if (ch == ")") paren_depth <- max(0L, paren_depth - 1L)
    if (ch == "[") bracket_depth <- bracket_depth + 1L
    if (ch == "]") bracket_depth <- max(0L, bracket_depth - 1L)
    if (ch == "{") brace_depth <- brace_depth + 1L
    if (ch == "}") brace_depth <- max(0L, brace_depth - 1L)
  }

  c(parts, paste0(current, collapse = "")) |>
    trimws()
}

# Parse names of the form:
#   BM_FindOrAddInsert<Backend::StdMap, int, int>/16
# The first template argument is the structure/backend. All remaining
# template arguments are joined into the combined key/value type label.
parse_benchmark_name <- function(name) {
  match <- regmatches(
    name,
    regexec("^([^<]+)<(.*)>/([0-9]+)$", name, perl = TRUE)
  )[[1]]

  if (length(match) != 4L) {
    return(NULL)
  }

  template_args <- split_cpp_template_args(match[[3]])
  if (length(template_args) < 2L) {
    return(NULL)
  }

  list(
    function_name = trimws(match[[2]]),
    structure = trimws(template_args[[1]]),
    combined_type = paste(template_args[-1], collapse = ", "),
    benchmark_size = as.numeric(match[[4]])
  )
}

safe_path_component <- function(x) {
  x <- gsub("::", "_", x, fixed = TRUE)
  x <- gsub("[^A-Za-z0-9._-]+", "_", x)
  x <- gsub("_+", "_", x)
  x <- gsub("^[_.]+|[_.]+$", "", x)
  if (!nzchar(x)) "unnamed" else x
}

# Google Benchmark normally emits ns/us/ms/s. Convert everything to ns first
# so that rings from different backends are compared on the same scale.
time_unit_to_ns <- c(ns = 1, us = 1e3, `µs` = 1e3, `μs` = 1e3,
                     ms = 1e6, s = 1e9)

choose_display_unit <- function(max_time_ns) {
  if (max_time_ns >= 1e9) {
    list(unit = "s", divisor = 1e9)
  } else if (max_time_ns >= 1e6) {
    list(unit = "ms", divisor = 1e6)
  } else if (max_time_ns >= 1e3) {
    list(unit = "us", divisor = 1e3)
  } else {
    list(unit = "ns", divisor = 1)
  }
}

plot_radial <- function(data, title, structure_colours) {
  # Put the fastest structure on the innermost ring, then order outward by
  # increasing time. Break ties by structure name for deterministic placement.
  data <- data[order(data$time_ns, as.character(data$structure)), , drop = FALSE]
  structures <- unique(as.character(data$structure))
  data$structure <- factor(data$structure, levels = names(structure_colours))

  display <- choose_display_unit(max(data$time_ns, na.rm = TRUE))
  data$plot_time <- data$time_ns / display$divisor
  data$ring <- seq_len(nrow(data)) * 1.5
  # Keep the original multiline label: structure name on the first line,
  # measured time and unit on the second line.
  data$ring_label <- paste0(
    as.character(data$structure), "\n",
    format(signif(data$plot_time, 4), trim = TRUE, scientific = FALSE),
    " ", display$unit
  )

  max_plot_time <- max(data$plot_time, na.rm = TRUE)
  if (!is.finite(max_plot_time) || max_plot_time <= 0) {
    max_plot_time <- 1
  }
  limit <- max_plot_time * 1.25
  ring_positions <- data$ring

  # Colours come from one global named palette created from every structure
  # in the input JSON. A structure therefore keeps the same colour in every
  # function/type plot and at every benchmark size.
  ring_colours <- structure_colours

  # A separate label panel guarantees that labels form one vertical column
  # to the LEFT of the wheel. Each structure gets exactly one row.
  label_data <- data
  label_count <- nrow(label_data)
  # Keep the labels in a compact, centered column beside the wheel. The old
  # fixed y-range made adjacent rows too close together, so multiline labels
  # overlapped. Scale the y-range with the number of structures: the column
  # uses about 30% of the wheel height for up to five labels, then expands only
  # as much as needed to preserve a readable minimum row gap.
  label_data$label_row <- rev(seq_len(label_count)) - (label_count + 1) / 2
  target_stack_fraction <- 0.30
  minimum_row_gap_fraction <- 0.07
  row_gap_fraction <- max(
    minimum_row_gap_fraction,
    target_stack_fraction / max(1, label_count - 1)
  )
  label_extent <- 1 / (2 * row_gap_fraction)

  label_plot <- ggplot(
    label_data,
    aes(x = 0, y = label_row, label = ring_label, color = structure)
  ) +
    geom_label(
      hjust = 0,
      fill = "white",
      label.size = 0.25,
      label.padding = grid::unit(0.10, "lines"),
      size = 2.7,
      fontface = "bold",
      show.legend = FALSE
    ) +
    scale_color_manual(values = ring_colours, guide = "none") +
    scale_x_continuous(limits = c(0, 1), expand = expansion(mult = 0)) +
    scale_y_continuous(
      limits = c(-label_extent, label_extent),
      expand = expansion(mult = 0)
    ) +
    coord_cartesian(clip = "off") +
    theme_void() +
    theme(
      plot.margin = margin(28, 0, 24, 8)
    )

  wheel_plot <- ggplot(data) +
    # Full circular tracks for each structure.
    geom_hline(yintercept = ring_positions, linewidth = 14, color = "#dfdfdf") +
    geom_hline(yintercept = ring_positions, linewidth = 13, color = "#f7f7f7") +
    # Structure-specific foreground arcs indicate elapsed time.
    geom_segment(
      aes(x = 0, xend = plot_time, y = ring, yend = ring),
      color = "#bf2c23", linewidth = 14.3, lineend = "round"
    ) +
    geom_segment(
      aes(x = 0, xend = plot_time, y = ring, yend = ring, color = structure),
      linewidth = 13, lineend = "round"
    ) +
    geom_point(
      aes(x = plot_time, y = ring), size = 6,
      color = "#7f323e", shape = 21, fill = "white", stroke = 1
    ) +
    geom_point(
      aes(x = plot_time, y = ring), size = 2.8,
      color = "#7f323e", shape = 21, fill = "white", stroke = 1
    ) +
    scale_color_manual(values = ring_colours, guide = "none") +
    scale_x_continuous(limits = c(0, limit), expand = expansion(mult = 0)) +
    scale_y_continuous(
      limits = c(0, max(ring_positions) + 1.1),
      expand = expansion(mult = 0)
    ) +
    coord_polar(theta = "x", start = 0, clip = "off") +
    theme_void() +
    theme(
      plot.title = element_text(hjust = 0.5, face = "bold"),
      plot.margin = margin(8, 18, 8, 0)
    ) +
    ggtitle(title)

  # Compose one wheel unit: a vertical label column on the left and the
  # corresponding radial visualization on the right.
  patchwork::wrap_plots(
    label_plot,
    wheel_plot,
    ncol = 2,
    widths = c(1.8, 3.0)
  )
}

read_google_benchmark_json <- function(file) {
  json <- jsonlite::fromJSON(file, simplifyDataFrame = TRUE)
  if (is.null(json$benchmarks)) {
    stop("The JSON file does not contain a top-level 'benchmarks' array.", call. = FALSE)
  }

  benchmarks <- tibble::as_tibble(json$benchmarks)
  required_columns <- c("name", "real_time", "time_unit")
  missing_columns <- setdiff(required_columns, names(benchmarks))
  if (length(missing_columns) > 0) {
    stop(
      "Missing required benchmark field(s): ",
      paste(missing_columns, collapse = ", "),
      call. = FALSE
    )
  }

  # Ignore aggregate rows (mean/median/stddev/cv). Individual repetition rows
  # are combined below using their median real_time.
  if ("run_type" %in% names(benchmarks)) {
    iteration_rows <- dplyr::filter(benchmarks, .data$run_type == "iteration")
    if (nrow(iteration_rows) > 0L) {
      benchmarks <- iteration_rows
    }
  }

  if ("error_occurred" %in% names(benchmarks)) {
    benchmarks <- dplyr::filter(benchmarks, is.na(.data$error_occurred) | !.data$error_occurred)
  }

  parsed <- purrr::map(benchmarks$name, parse_benchmark_name)
  valid <- !vapply(parsed, is.null, logical(1))
  if (!any(valid)) {
    stop(
      "No benchmark names matched '<function><template arguments>/<number>'.",
      call. = FALSE
    )
  }

  if (any(!valid)) {
    warning(sum(!valid), " benchmark row(s) had an unsupported name and were skipped.")
  }

  benchmarks <- benchmarks[valid, , drop = FALSE]
  parsed_data <- dplyr::bind_rows(lapply(parsed[valid], tibble::as_tibble))
  benchmarks <- dplyr::bind_cols(benchmarks, parsed_data)

  units <- as.character(benchmarks$time_unit)
  units[units %in% c("µs", "μs")] <- "us"
  unknown_units <- setdiff(unique(units), names(time_unit_to_ns))
  if (length(unknown_units) > 0L) {
    stop("Unsupported time unit(s): ", paste(unknown_units, collapse = ", "), call. = FALSE)
  }

  benchmarks$time_ns <- as.numeric(benchmarks$real_time) *
    unname(time_unit_to_ns[units])

  benchmarks |>
    dplyr::filter(is.finite(.data$time_ns), .data$time_ns >= 0) |>
    # Multiple repetitions of one benchmark are represented by one ring.
    dplyr::group_by(.data$function_name, .data$combined_type,
                    .data$structure, .data$benchmark_size) |>
    dplyr::summarise(
      time_ns = stats::median(.data$time_ns, na.rm = TRUE),
      repetition_count = dplyr::n(),
      .groups = "drop"
    )
}

main <- function() {
  args <- commandArgs(trailingOnly = TRUE)
  if (length(args) < 1L) {
    stop(
      "Usage: Rscript plot_google_benchmark_radial.R <benchmark.json> [output_directory]",
      call. = FALSE
    )
  }

  input_file <- normalizePath(args[[1]], mustWork = TRUE)
  output_root <- if (length(args) >= 2L) args[[2]] else "plots"
  json_base_name <- tools::file_path_sans_ext(basename(input_file))
  json_directory <- file.path(output_root, safe_path_component(json_base_name))

  benchmark_data <- read_google_benchmark_json(input_file)
  if (nrow(benchmark_data) == 0L) {
    stop("No usable benchmark measurements were found in the JSON file.", call. = FALSE)
  }

  # Assign colours once for the entire input file, not independently per
  # wheel. Sorting the names makes the mapping deterministic between runs.
  all_structures <- sort(unique(as.character(benchmark_data$structure)))
  structure_colours <- stats::setNames(
    grDevices::hcl.colors(length(all_structures), palette = "Dark 3"),
    all_structures
  )

  groups <- split(
    benchmark_data,
    interaction(benchmark_data$function_name, benchmark_data$combined_type,
                drop = TRUE, lex.order = TRUE)
  )

  for (group_data in groups) {
    function_name <- unique(group_data$function_name)[[1]]
    combined_type <- unique(group_data$combined_type)[[1]]

    # Each benchmark argument (/8, /16, /32, ...) gets a separate radial wheel.
    sizes <- sort(unique(group_data$benchmark_size))
    wheel_plots <- lapply(sizes, function(size) {
      wheel_data <- group_data[group_data$benchmark_size == size, , drop = FALSE]
      plot_radial(
        wheel_data,
        paste0("Benchmark size: ", format(size, scientific = FALSE)),
        structure_colours
      )
    })

    # Arrange up to four radial wheels across each row.
    n_columns <- min(4L, length(wheel_plots))
    n_rows <- ceiling(length(wheel_plots) / n_columns)
    combined_plot <- patchwork::wrap_plots(wheel_plots, ncol = n_columns) +
      patchwork::plot_annotation(
        title = paste0(function_name, " — ", combined_type),
        subtitle = paste0(
          "Each wheel is one benchmark size; each ring is a structure/backend. ",
          "Values show median real_time across ",
          "repetition(s), converted to a common unit within each wheel."
        )
      )

    # Group plots by input JSON, then function name:
    # <output>/<json_baseName>/<function_name>/
    #   <function_name>_<combined_type>.png
    function_directory <- file.path(json_directory, safe_path_component(function_name))
    dir.create(function_directory, recursive = TRUE, showWarnings = FALSE)
    output_file <- file.path(
      function_directory,
      paste0(
        safe_path_component(function_name), "_",
        safe_path_component(combined_type), ".png"
      )
    )

    ggplot2::ggsave(
      filename = output_file,
      plot = combined_plot,
      width = 1150 * n_columns,
      height = 900 * n_rows,
      units = "px",
      dpi = 160,
      bg = "white"
    )
    message("Wrote: ", output_file)
  }

  message("Finished plotting ", length(groups), " function/type combination(s).")
}

main()
