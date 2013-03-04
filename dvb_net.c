/*  
 *  dvb net_device
 */
#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

static struct net_device* dev;

static int dvb_net_tx(struct sk_buff *skb, struct net_device *dev)
{
    printk(KERN_INFO "dvbnet tx");
    /* dev_kfree_skb(skb); */
    return NETDEV_TX_OK;
}

static void dvb_net_set_multicast_list (struct net_device *dev)
{
    /* struct dvb_net_priv *priv = netdev_priv(dev); */
    /* schedule_work(&priv->set_multicast_list_wq); */
}

static int dvb_net_set_mac (struct net_device *dev, void *p)
{
    /* struct dvb_net_priv *priv = netdev_priv(dev); */
    /* struct sockaddr *addr=p; */

    /* memcpy(dev->dev_addr, addr->sa_data, dev->addr_len); */

    /* if (netif_running(dev)) */
    /*     schedule_work(&priv->restart_net_feed_wq); */

    return 0;
}

static int dvb_net_open(struct net_device *dev)
{
    printk(KERN_INFO "dvbnet open.\n");
    /* struct dvb_net_priv *priv = netdev_priv(dev); */

    /* priv->in_use++; */
    /* dvb_net_feed_start(dev); */

    /* memcpy(dev->dev_addr, "\0ABCD0", ETH_ALEN); */
    /* netif_start_queue(dev); */

    return 0;
}

static int dvb_net_stop(struct net_device *dev)
{
    printk(KERN_INFO "dvbnet stop.\n");
    /* struct dvb_net_priv *priv = netdev_priv(dev); */

    /* priv->in_use--; */
    /* return dvb_net_feed_stop(dev); */
    return 0;
}

/*
 * Configuration changes (passed on by ifconfig)
 */
static int dvb_config(struct net_device *dev, struct ifmap *map)
{
    if (dev->flags & IFF_UP) /* can't act on a running interface */
        return -EBUSY;

    /* Don't allow changing the I/O address */
    if (map->base_addr != dev->base_addr) {
        printk(KERN_WARNING "dvbnet: Can't change I/O address\n");
        return -EOPNOTSUPP;
    }

    /* Allow changing the IRQ */
    if (map->irq != dev->irq) {
        dev->irq = map->irq;
        /* request_irq() is delayed to open-time */
    }

    /* ignore other fields */
    return 0;
}

/*
 * Ioctl commands 
 */
int dvb_do_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
    printk(KERN_DEBUG "dvbnet ioctl\n");
    return 0;
}

static const struct net_device_ops dvb_netdev_ops = {
    .ndo_open               = dvb_net_open,
    .ndo_stop               = dvb_net_stop,
    .ndo_start_xmit         = dvb_net_tx,
    /* .ndo_do_ioctl           = dvb_do_ioctl, */
    /* .ndo_set_rx_mode        = dvb_net_set_multicast_list, */
    /* .ndo_set_mac_address    = dvb_net_set_mac, */
    /* .ndo_change_mtu         = eth_change_mtu, */
    /* .ndo_set_config         = dvb_config, */

    // cause SIOCSIFFLAGS: Cannot assign requested address, (ifconfig dvb0 ip)
    /* .ndo_validate_addr      = eth_validate_addr, */
};
 
static const struct header_ops dvb_header_ops = {
    .create     = eth_header,
    .parse      = eth_header_parse,
    .rebuild    = eth_rebuild_header,
};

static void dvb_net_setup(struct net_device *dev)
{
    printk(KERN_INFO "dvbnet setup.\n");

    ether_setup(dev);
    /* dev->header_ops     = &dvb_header_ops; */
    dev->netdev_ops     = &dvb_netdev_ops;
    /* dev->mtu            = 4096; */
    /* dev->flags |= IFF_NOARP; */
}

static __init int dvb_net_init_module(void)
{
    printk(KERN_INFO "dvbnet init.\n");

    //struct net_device *dev;
    int err = 0;
    dev = alloc_netdev(0, "dvb%d", dvb_net_setup);
    if (!dev) {
        printk(KERN_ERR "dvbnet alloc netdev err\n");
        goto init_module_out;
    }

    err = register_netdev(dev);
    if (err) {
        printk(KERN_ERR "dvbnet register err\n");
        goto init_module_out;
    }

    return 0;

init_module_out:
    if (dev) 
        free_netdev(dev);

    return err;
}

static __exit void dvb_net_cleanup_module(void)
{
    printk(KERN_INFO "dvb net cleanup\n");
    unregister_netdev(dev);
}

module_init(dvb_net_init_module);
module_exit(dvb_net_cleanup_module);
