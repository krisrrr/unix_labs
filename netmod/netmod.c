#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h> /* printk */
#include <linux/slab.h> /* kmalloc */
#include <linux/types.h>
#include <linux/netdevice.h> /* struct device */
#include <linux/etherdevice.h>
#include <linux/skbuff.h>

MODULE_LICENSE("GPL");

struct net_device *bhole_ndev;
typedef struct { unsigned int requests; } MY_DRIVERDATA;

int my_net_open (struct net_device *dev) {
    memcpy(dev->dev_addr,"\0USKA",ETH_ALEN);
    printk("my_net_open called\n");
    netif_start_queue (dev);
    return 0;
}

int my_net_release (struct net_device *dev) {
    printk ("my_net_release called\n");
    netif_stop_queue(dev);
    return 0;
}

static int my_net_xmit (struct sk_buff *skb,
                         struct net_device *dev) {
    netif_trans_update(dev);
    dev->stats.tx_packets += 1;
    dev->stats.tx_bytes += skb->len;
    printk ("my_net drops your package\n");
    dev_kfree_skb(skb);
    return 0;
}

static const struct net_device_ops my_netdev_ops = {
        .ndo_open = my_net_open,
        .ndo_stop = my_net_release,
        .ndo_start_xmit = my_net_xmit,
};

int my_net_init_module (void) {
    bhole_ndev = alloc_etherdev( sizeof( MY_DRIVERDATA ) );
    bhole_ndev->netdev_ops = &my_netdev_ops;
    strcpy (bhole_ndev->name, "myNetDv");
    return register_netdev( bhole_ndev );
}

void my_net_cleanup (void) {
    printk ("<0> Cleaning Up the Module\n");
    unregister_netdev (bhole_ndev);
    free_netdev( bhole_ndev);
    return;
}

module_init (my_net_init_module);
module_exit (my_net_cleanup);
