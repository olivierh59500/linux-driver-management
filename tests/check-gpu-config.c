/*
 * This file is part of linux-driver-management.
 *
 * Copyright © 2016-2018 Linux Driver Management Developers, Solus Project
 *
 * linux-driver-management is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#define _GNU_SOURCE

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <umockdev.h>

#include "ldm-private.h"
#include "ldm.h"
#include "util.h"

DEF_AUTOFREE(UMockdevTestbed, g_object_unref)

#define NV_MOCKDEV_FILE TEST_DATA_ROOT "/nvidia1060.umockdev"
#define OPTIMUS_MOCKDEV_FILE TEST_DATA_ROOT "/optimus765m.umockdev"
#define DESKTOP_NVIDIA_MOCKDEV_FILE TEST_DATA_ROOT "/desktop-nvidia-intel.umockdev"

static UMockdevTestbed *create_bed_from(const char *mockdevname)
{
        UMockdevTestbed *bed = NULL;

        bed = umockdev_testbed_new();
        fail_if(!umockdev_testbed_add_from_file(bed, mockdevname, NULL),
                "Failed to create device: %s",
                mockdevname);

        return bed;
}

/**
 * This test will deal with the very basic _SIMPLE type GPU
 */
START_TEST(test_gpu_config_simple)
{
        g_autoptr(LdmManager) manager = NULL;
        autofree(UMockdevTestbed) *bed = NULL;
        g_autoptr(LdmGPUConfig) gpu = NULL;
        guint n_gpu = 0;

        bed = create_bed_from(NV_MOCKDEV_FILE);
        manager = ldm_manager_new(0);

        gpu = ldm_gpu_config_new(manager);
        fail_if(!gpu, "Failed to create GPUConfig");

        n_gpu = ldm_gpu_config_count(gpu);
        fail_if(n_gpu != 1, "Invalid number of GPUs (%u) - expected %u", n_gpu, 1);

        fail_if(ldm_gpu_config_get_gpu_type(gpu) != LDM_GPU_TYPE_SIMPLE, "GPU type isn't simple");
}
END_TEST

/**
 * This test helps us ensure we can always reliably detect an Optimus system
 */
START_TEST(test_gpu_config_optimus)
{
        g_autoptr(LdmManager) manager = NULL;
        autofree(UMockdevTestbed) *bed = NULL;
        g_autoptr(LdmGPUConfig) gpu = NULL;
        guint n_gpu = 0;

        bed = create_bed_from(OPTIMUS_MOCKDEV_FILE);
        manager = ldm_manager_new(0);

        gpu = ldm_gpu_config_new(manager);
        fail_if(!gpu, "Failed to create GPUConfig");

        n_gpu = ldm_gpu_config_count(gpu);
        fail_if(n_gpu != 2, "Invalid number of GPUs (%u) - expected %u", n_gpu, 2);

        fail_if(!ldm_gpu_config_has_type(gpu, LDM_GPU_TYPE_HYBRID),
                "Failed to detect hybrid graphics");
        fail_if(!ldm_gpu_config_has_type(gpu, LDM_GPU_TYPE_OPTIMUS), "Failed to detect Optimus");
}
END_TEST

/**
 * This test has both an Intel onboard GPU and an active NVIDIA GPU over PCI,
 * we must ensure we only detect an NVIDIA GPU configuration, in the SIMPLE
 * arrangement (i.e. not Optimus!)
 */
START_TEST(test_gpu_config_desktop_nvidia)
{
        g_autoptr(LdmManager) manager = NULL;
        autofree(UMockdevTestbed) *bed = NULL;
        g_autoptr(LdmGPUConfig) gpu = NULL;
        guint n_gpu = 0;

        bed = create_bed_from(DESKTOP_NVIDIA_MOCKDEV_FILE);
        manager = ldm_manager_new(0);

        gpu = ldm_gpu_config_new(manager);
        fail_if(!gpu, "Failed to create GPUConfig");

        n_gpu = ldm_gpu_config_count(gpu);
        fail_if(n_gpu != 2, "Invalid number of GPUs (%u) - expected %u", n_gpu, 2);

        fail_if(ldm_gpu_config_has_type(gpu, LDM_GPU_TYPE_HYBRID),
                "Incorrectly detected hybrid graphics!");
        fail_if(ldm_gpu_config_has_type(gpu, LDM_GPU_TYPE_OPTIMUS),
                "Incorrectly detected Optimus!");

        fail_if(ldm_gpu_config_get_gpu_type(gpu) != LDM_GPU_TYPE_SIMPLE,
                "Config type should be simple ONLY");
}
END_TEST

/**
 * Standard helper for running a test suite
 */
static int ldm_test_run(Suite *suite)
{
        SRunner *runner = NULL;
        int n_failed = 0;

        runner = srunner_create(suite);
        srunner_run_all(runner, CK_VERBOSE);
        n_failed = srunner_ntests_failed(runner);
        srunner_free(runner);

        return n_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static Suite *test_create(void)
{
        Suite *s = NULL;
        TCase *tc = NULL;

        s = suite_create(__FILE__);
        tc = tcase_create(__FILE__);
        suite_add_tcase(s, tc);

        tcase_add_test(tc, test_gpu_config_simple);
        tcase_add_test(tc, test_gpu_config_optimus);
        tcase_add_test(tc, test_gpu_config_desktop_nvidia);

        return s;
}

int main(__ldm_unused__ int argc, __ldm_unused__ char **argv)
{
        return ldm_test_run(test_create());
}

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
