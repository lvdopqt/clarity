extern "C" long write(int fd, const void* buf, unsigned long count);

void print_str(const char* str) {
    unsigned long len = 0;
    while (str[len] != '\0') ++len;
    write(1, str, len);
}

void print_float(float x) {
    if (x < 0) {
        print_str("-");
        x = -x;
    }
    int int_part = (int)x;
    float frac_part = x - int_part;
    char buf[16];
    int i = 0;
    if (int_part == 0) {
        buf[i++] = '0';
    } else {
        while (int_part > 0 && i < 15) {
            buf[i++] = '0' + (int_part % 10);
            int_part /= 10;
        }
    }
    for (int j = i - 1; j >= 0; --j) write(1, &buf[j], 1);
    write(1, ".", 1);
    for (int d = 0; d < 3; ++d) {
        frac_part *= 10;
        int digit = (int)frac_part;
        char c = '0' + digit;
        write(1, &c, 1);
        frac_part -= digit;
    }
}

#include <cmath>

#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
inline float fast_sqrt(float x) {
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
}
// Placeholder for x86 log10 (needs proper implementation)
inline float fast_log10(float x) {
    return std::log10(x); // Using std::log10 for now on x86
}
#elif defined(__ARM_NEON__)
#include <arm_neon.h>
inline float fast_sqrt(float x) {
    float32x2_t v = vdup_n_f32(x);
    v = vsqrt_f32(v);
    return vget_lane_f32(v, 0);
}
// Placeholder for ARM log10 (needs proper implementation)
inline float fast_log10(float x) {
    return std::log10(x); // Using std::log10 for now on ARM
}
#else
inline float fast_sqrt(float x) {
    return std::sqrt(x);
}
inline float fast_log10(float x) {
    return std::log10(x);
}
#endif

float compute_rms(const float* frame, size_t frame_size) {
    float sum = 0.0f;
    for (size_t i = 0; i < frame_size; ++i) {
        sum += frame[i] * frame[i];
    }
    return fast_sqrt(sum / frame_size);
}

int main() {
    float signal_data[] = {
        0.1f, 0.2f, -0.15f, 0.05f, 0.25f, -0.2f, 0.1f, -0.05f,
        0.15f, 0.2f, -0.1f, 0.0f, 0.3f, -0.25f, 0.05f, -0.1f,
    };
    size_t signal_size = sizeof(signal_data) / sizeof(float);
    const int frame_size = 320;
    float frame_energies[signal_size / frame_size];
    int num_frames = signal_size / frame_size;

    for (int i = 0; i < num_frames; ++i) {
        const float* frame = signal_data + i * frame_size;
        frame_energies[i] = compute_rms(frame, frame_size);
    }

    float mean = 0.0f;
    for (int i = 0; i < num_frames; ++i) {
        mean += frame_energies[i];
    }
    mean /= num_frames;

    float variance = 0.0f;
    for (int i = 0; i < num_frames; ++i) {
        variance += (frame_energies[i] - mean) * (frame_energies[i] - mean);
    }
    variance /= num_frames;

    float snr_estimate = variance / (mean * mean);
    float snr_db = 10.0f * fast_log10(snr_estimate);

    print_str("SNR estimate is ");
    print_float(snr_db);
    print_str("dB\n");

    return 0;
}